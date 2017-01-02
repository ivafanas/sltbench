# sltbench

[![Build Status](https://travis-ci.org/ivafanas/sltbench.svg)](https://travis-ci.org/ivafanas/sltbench)
[![Build status](https://ci.appveyor.com/api/projects/status/s0q3gnqprrqsahda?svg=true)](https://ci.appveyor.com/project/ivafanas/sltbench)

C++ benchmark tool. Practical, stable and fast performance testing framework.
- practical - came up from daily production tasks. Usability matters
- stable - correctness and reproducibility is a goal
- fast - if results are stable, they should be produced as soon as possible

This tool is trained to gain about 5% measure stability (depends on function, input and context).
Of course, it cannot strongly guarantee 5% stability (some functions are unstable itself, nothing helps), but it tends to.


# Build instructions


## Linux

```bash
# clone git repo
git clone https://github.com/ivafanas/sltbench.git
cd sltbench
git submodule init
git submodule update
cd ..

# create temp directories for building
mkdir build install

# generate, build, install
cd build
cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install -DBUILD_TESTS=ON ${PWD}/../sltbench -DCMAKE_BUILD_TYPE=Release
make install

# run unittests (just to make shure)
./sltbench_unittests

# prepared sltbench lib is here:
# ${PWD}/../install
```

## Windows

```bat
git clone https://github.com/ivafanas/sltbench.git
cd sltbench
git submodule init
git submodule update
cd ..

mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=..\install -DBUILD_TESTS=ON ..\sltbench
msbuild.exe INSTALL.vcxproj /p:Configuration=Release
```


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
- input values are presented as `std::vector`
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

static const std::vector<size_t> my_args = { 100000, 110000, 120000 };

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


## Function with input values lazy generator

If input value consumes a lot memory and
whole values set doesnot fit into RAM,
lazy generator should be used.

Requirements:
- generator is constructible from `(int argc, char** argv)`
- generator has inner typedef `ArgType`
- operator `std::ostream <<` is defined for `ArgType`
- `ArgType` has copy constructor or (better) move constructor
- generator has member function `ArgType Generate()`
- `Generate` member function either returns value for testing
either throws `sltbench::StopGenerationException`

```c++
class Generator
{
public:
	typedef HugeMemoryConsumingStruct ArgType;

	MyArgsGenerator(int argc, char **argv) { /* ... */ }

	ArgType Generate()
	{
		bool continue_generation = /*...*/;
		if (!continue_generation)
			throw sltbench::StopGenerationException();

		return HugeMemoryConsumingStruct(/*...*/);
	}
};

void my_function(const HugeMemoryConsumingStruct& arg)
{
	/*
	 * process arg here
	 * ...
	 */
}

SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(my_function, Generator);

SLTBENCH_MAIN();
```


## Function with fixture and input values lazy generator

If input value consumes a lot memory and
whole values set doesnot fit into RAM,
lazy generator should be used.

Requirements:
- fixture class is default constructible
- fixture has inner typedef `Type`
- fixture has member function `Type& Setup(const generator::ArgType&)`
- fixture has member function `void TearDown()`
- generator is constructible from `(int argc, char** argv)`
- generator has inner typedef `ArgType`
- operator `std::ostream <<` is defined for `ArgType`
- `ArgType` has copy constructor or (better) move constructor
- generator has member function `ArgType Generate()`
- `Generate` member function either returns value for testing
either throws `sltbench::StopGenerationException`

```c++
class Generator
{
public:
	typedef HugeMemoryConsumingStruct ArgType;

	Generator(int argc, char **argv) { /* ... */ }

	ArgType Generate()
	{
		bool continue_generation = /*...*/;
		if (!continue_generation)
			throw sltbench::StopGenerationException();

		return HugeMemoryConsumingStruct(/*...*/);
	}
};

class Fixture
{
public:
	typedef std::vector<size_t> Type;

	Fixture() {}

	Type& SetUp(const Generator::ArgType& arg)
	{
		/* ... */
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

void my_function(Fixture::Type& fix, const HugeMemoryConsumingStruct& arg)
{
	/*
	 * process fix and arg here
	 * ...
	 */
}

SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(my_function, Generator);

SLTBENCH_MAIN();
```


# Heatup

Benchmark "heatups" OS and hardware before timing. This process takes about
5 seconds (may be less). If you do not want to wait, just add a `--heatup off`
command line option to your benchmark. Be careful, disabling heatup may lead to
unstable results for the first test cases.
```bash
# Skip heatup stage, run tests ASAP.
# May lead to unstable first test cases.
./my_benchmark.exe --heatup off
```


# How to benchmark

Producing stable performance results is a quite tricky task.
First of all, make sure that (**all points are critical for stable performance results**):
* The computer you are using is as much clean as possible.
No browser is running, no background updates, no deamons / services (like databases, antivirus, background email fetching and so on).
* There is no other users logged in to the computer.
* Intel SpeedStep and Intel Turbo technologies are turned off (check your BIOS settings).
* You are NOT running from Visual Studio or another IDE (open separate console).
* The benchmark process is pinned to a single CPU core and this is not #0 core.
It is important to get rid of task switching between CPU cores, so pin your process.
In some systems OS preferably occupies core #0, benchmark should avoid to struggle with OS for resources.
For windows follow [this instructions](http://www.howtogeek.com/121775/how-to-force-windows-applications-to-use-a-specific-cpu/) to pin process to core.
For linux use `taskset` and `KMP_AFFINITY`.


# How to filter benchmarking items

Sometimes limited set of functions need to be benchmarked per run
(for example, if you are tuning one function, you do not need to measure
all functions in benchmark per run, just the one you are interested in).
It is possible to set filter by benchmark name:

This example shows how to run all functions wich name contains `MyFunction` substring:
```sh
./my_benchmark.exe --filter=.*MyFunction.*
```

Filter regular expression format corresponds to [basic case of std::regex](http://en.cppreference.com/w/cpp/regex/syntax_option_type).


# License

sltbench is released under the Apache 2.0 license
