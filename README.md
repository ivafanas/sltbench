# sltbench
C++ benchmark tool. Practical, stable and fast.
- practical - arised from daily production tasks. Usability matters
- stable - correctness and reproducibility is a goal
- fast - if results are stable, they should be produced as soon as possible

This tool is learning to gain about 5% measure stability (depends on function, input and context).
Of course, it cannot strongly guarantee 5% stability (some functions are unstable itself, nothing helps), but it tends to.


# Examples


## Simple function

Register benchmark for function

```c++
void my_function()
{
	std::vector<size_t> v(100000, 0);
	std::sort(v.begin(), v.end());
}

SLTBENCH_FUNCTION(my_function);

SLTBENCH_MAIN();
```



## Function with many input values

What if `100000` size is not enough,
and we want to benchmark function for several input values?
Well, it is quite easy.

Requirements:
- function gets input value by const reference
- input values are presented as std::vector
- type of input value provides `std::ostream <<` operator (if not, just implement it)

```c++
void my_function(const size_t& count)
{
	std::vector<size_t> vec(count, 0);
	std::sort(vec.begin(), vec.end());
}

static const std::vector<size_t> my_args{ 100000, 110000, 120000, 130000, 140000, 150000 };

SLTBENCH_FUNCTION_WITH_ARGS(my_function, my_args);

SLTBENCH_MAIN();
```


## Function with fixture

What if initialization code like `std::vector<size_t> v(count, 0)`
in the example above should not be benchmarked,
and we are interested in performance of `std::sort` only?
Then, fixture should be used.

Requirements:
- fixture class is default constructible
- fixture has inner typedef `Type`
- fixture has member function `Type& Setup()`
- fixture has member function `void TearDown()`

```c++
class MyFixture
{
public:
	typedef std::vector<size_t> Type;

	MyFixture() {}	
	
	Type& SetUp()
	{
		fixture_.resize(100000, 0);
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};


void my_function(MyFixture::Type& fix)
{
	std::sort(fix.begin(), fix.end());
}

SLTBENCH_FUNCTION_WITH_FIXTURE(my_function, MyFixture);

SLTBENCH_MAIN();
```

Be careful, `SetUp` and `TearDown` methods are called per each run.
If function execution time is small enough (which leads to huge number of iterations)
and `SetUp` and `TearDown` are expensive, benchmark may produce results for a long long time.


## Function with input values generator

What if input values are not known at compile time?
It is possible to pass input values set from commad line or (better) file
with filename given in command line parameters.
Input values generator is designed for this purpose.

Requirements:
- generator is default constructible
- generator has inner typedef `ArgType`
- operator `std::ostream <<` is defined for `ArgType`
- generator has member function `std::vector<ArgType> Generate(int argc, char **argv)`

```c++

class MyArgsGenerator
{
public:
	typedef size_t ArgType;

	MyArgsGenerator() {}

	std::vector<ArgType> Generate(int argc, char **argv)
	{
		std::vector<ArgType> values;
		// read input values from argc, argv
		// or from file which name given in arc, argv
		// or as you wish...
		return values;
	}
};

void my_function(const MyArgsGenerator::ArgType& arg)
{
	std::vector<size_t> vec(arg, 0);
	std::sort(vec.begin(), vec.end());
}

SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(my_function, MyArgsGenerator);

SLTBENCH_MAIN();
```

`Generate` method is called once per function,
all input values returned by generator are copied to internal structure.


## Function with fixture and many input values

This example is for the case when we need both
initialization and several input values known at compile time.

Requirements:
- fixture class is default constructible
- fixture has inner typedef `Type`
- fixture has member function `Type& Setup(const T&)`
- fixture has member function `void TearDown()`
- `T` is a type of input value
- operator `std::ostream <<` is defined for type T (if not, just implement it)

```c++
class MyFixture
{
public:
	typedef std::vector<size_t> Type;

	MyFixture() {}	
	
	Type& SetUp(const size_t& arg)
	{
		fixture_.resize(arg, 0);
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

void my_function(MyFixture::Type& fix, const size_t& arg)
{
	std::sort(fix.begin(), fix.end());
}

static const std::vector<NSortFunctionArg> my_args = { 100000, 110000, 120000 };

SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(my_function, MyFixture, my_args);

SLTBENCH_MAIN();
```


## Function with fixture and input values generator

This example is for the case when we need both
initialization and several input values known at runtime.

Requirements:
- fixture class is default constructible
- fixture has inner typedef `Type`
- fixture has member function `Type& Setup(const generator::ArgType&)`
- fixture has member function `void TearDown()`
- generator is default constructible
- generator has inner typedef `ArgType`
- operator `std::ostream <<` is defined for `ArgType`
- generator has member function `std::vector<ArgType> Generate(int argc, char **argv)`

```c++
class MyArgsGenerator
{
public:
	typedef size_t ArgType;

	MyArgsGenerator() {}

	std::vector<ArgType> Generate(int argc, char **argv)
	{
		std::vector<ArgType> values;
		// init values here ...
		return values;
	}
};

class MyFixture
{
public:
	typedef std::vector<size_t> Type;

	MyFixture() {}	
	
	Type& SetUp(const MyArgsGenerator::ArgType& arg)
	{
		fixture_.resize(arg, 0);
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

void my_function(MyFixture::Type& fix, const MyArgsGenerator::ArgType& arg)
{
	std::sort(fix.begin(), fix.end());
}

SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(my_function, MyFixture, MyArgsGenerator);

SLTBENCH_MAIN();
```


# How to benchmark

Producing stable performance results is a quite tricky task.
First of all, make sure that (**all points are critical for stable performance results**):
* The computer you are using is as much clean as possible.
No browser is running, no background updates, no deamons / services (like databases, antivirus, background email fetching and so on).
* There is no other users logged in to the computer.
* Intel SpeedStep and Intel Turbo technologies are turned off (check your BIOS settings).
* You are NOT running from Visual Studio (F5) or another IDE (open separate console).
* The benchmark process is pinned to a single CPU core and this is not #0 core.
It is important to get rid of task switching between CPU cores, so pin your process.
In some systems OS preferably occupies core #0, benchmark should avoid to struggle with OS for resources.
For windows follow [this instructions](http://www.howtogeek.com/121775/how-to-force-windows-applications-to-use-a-specific-cpu/) to pin process to core.
For linux use `taskset` and `KMP_AFFINITY`.



