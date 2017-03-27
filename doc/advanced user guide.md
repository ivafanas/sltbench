# Advanced user guide

## Preventing benchmark code being optimized away

Let's look for the example below.

```
void my_function()
{
	size_t rv;
	for (size_t i = 0; i < 100000; ++i)
		rv += i;
}
SLTBENCH_FUNCTION(my_function);
```

Compiler will likely omptimize out calculation inside `my_function`.
To prevent optimization use `DoNotOptimize` function:

```
void my_function()
{
	size_t rv;
	for (size_t i = 0; i < 100000; ++i)
		sltbench::DoNotOptimize(rv += i);
}
SLTBENCH_FUNCTION(my_function);
```

`DoNotOptimize` will force compiler to store expression result, but
computations inside expression in `DoNotOptimize` can be optimized.
Keep in mind that `DoNotOptimize` leads to memory barrier, which is
an performance overhead. Make shure that memory barrier overhead is
acceptable for your measurement.


## How to cutomize output

To customize benchmark output:
* Implement custom reporter class and
* Register it in the benchmark config.

```c++
// Custom reporter has to be inherited from IReporter interface
class CustomReporter : public sltbench::reporter::IReporter
{
public:
	CustomReporter() = default;
	~CustomReporter() override = default;

public:
	//! Called once before any test run
	void ReportBenchmarkStarted() override
	{
		// ...
	}

	//! Called once after all tests run
	void ReportBenchmarkFinished() override
	{
		// ...
	}

	//! Report about the new test case measure result
	void Report(
		const std::string& name,
		const std::string& params,
		sltbench::Verdict verdict,
		std::chrono::nanoseconds timing_result) override
	{
		// ...
	}

	//! Called when warning detected
	void ReportWarning(sltbench::RunWarning warning) override
	{
		// ...
	}
};

// Register custom reporter in the benchmark config.
SLTBENCH_CONFIG().SetReporter(std::make_unique<CustomReporter>());
```


## How to deal with IMPRECISE status

`IMPRECISE` is the status of test case performance measurement. It means that
sltbench can not even try to achieve correct and reproducible results. 

This happened when:
* testing function execetion time is extremely small (about timer
resolution x100) and
* testing function requires fixtures.

sltbench uses multicalls to measure functions with extremely small
execution time (where timer resolution is a significant tolerant error).
If testing function requires fixture (which is mutable by definition),
sltbench can not use multicalls - it is incorrect for mutable input.
So, sltbench warns user about imprecise results via `status` field.

Possible ways to fix that:


### Method 1. If your fixture is naturally immutable, make it an argument.

Likely `IMPRECISE`:
```
std::list<size_t> make_my_fixture()
{
	return {1, 2, 3};
}

// testing function with possibly extra small execution time
void my_fast_pivot(std::list<size_t>& fix)
{
	// let's propose, compiler will not optimize this code out :)
	int pivot = (fix.front() + fix.back()) / 2;
}

// benchmark is likely IMPRECISE - small execution time with fixtures 
// is hard to measure in a correct way.
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(my_fast_pivot, make_my_fixture);
```

`std::list<size_t>` is immutable here and can be made an argument.

`ok`:
```
std::ostream& operator <<(std::ostream& os, const std::list<size_t>& arg)
{
	// do not forget to implement operator << for complex arguments
	// ...
	return os;
}

// testing function with possibly extra small execution time
void my_fast_pivot(const std::list<size_t>& arg)
{
	// let's propose, compiler will not optimize this code out :)
	int pivot = (l.front() + l.back()) / 2;
}

static const std::vector<std::list<size_t>> my_args{{1, 2, 3}};

// benchmark is likely ok - sltbench can use multicall measure algorithm here
SLTBENCH_FUNCTION_WITH_ARGS(my_fast_pivot, my_args);
```


### Method 2. Manually increase iterations count inside testing function.

Do it very very carefully, make sure your fixture and algorithm allows such
modification.

likely `IMPRECISE`:
```
std::list<size_t> make_my_fixture()
{
	return {1, 2, 3};
}

// testing function with possibly extra small execution time
void my_fast_push_back(std::list<size_t>& fix)
{
	fix.push_back(1u);
}

// benchmark is likely IMPRECISE - small execution time with fixtures 
// is hard to measure in a correct way.
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(my_fast_push_back, make_my_fixture);
```

`ok`:
```
std::list<size_t> make_my_fixture()
{
	return {1, 2, 3};
}

// testing function with increased execution time
void BM_my_fast_push_back(std::list<size_t>& fix)
{
	// increase this value to achieve big enough
	// BM_my_fast_push_back execution time.
	const size_t calls_count = 1000u;
	for (size_t i = 0; i < calls_count; ++i)
		my_fast_push_back(fix);
}

// benchmark is likely ok - function execution should be big enough.
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(BM_my_fast_push_back, make_my_fixture);
```

Method 2 is definitely not ideal: at least, overhead on `for` loop and
function calls should be kept in mind.
