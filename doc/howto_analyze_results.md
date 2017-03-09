# How to analyse results

Sltbench prints the following results for each test case:
* `benchmark` - name of the test
* `arg` - string representation of the test case argument (if any)
* `status` - measure status
* `time(ns)` - measure result in nanoseconds

Output example for `console` reporter:
```
benchmark                      arg                      status               time(ns)
InsertToSetSorted              1000                     ok                    2052683
InsertToSetSorted              10000                    ok                   36229724
InsertToSetSorted              100000                   ok                  361036546
Shuffle_Fixture                1/1                      IMPRECISE                1463
Shuffle_Fixture                2/1                      IMPRECISE                1463
SimpleFunction_SHOULD_THROW                             CRASHED                     0
SimpleShortFunction                                     ok                        776
```
Other reporter options documented [here] (commandlineoptions.md).


## Statuses

`status` is one of the `{ok, CRASHED, IMPRECISE}`
* `ok` - benchmark has been run and sltbench did all he can to achieve
correct and reproducible result.
* `CRASHED` - an exception has been thrown during testing. `time(ns)` for this
test case will be set to zero.
* `IMPRECISE` - benchmark has been run, but it is hard to guarantee
correct and reproducible performance results for test case.


### How to deal with IMPRECISE status

`IMPRECISE` means that sltbench can not try to achieve correct and
reproducible results. 

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


#### Method 1. If your fixture is naturally immutable, make it an argument.

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


#### Method 2. Manually increase iterations count inside testing function.

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

