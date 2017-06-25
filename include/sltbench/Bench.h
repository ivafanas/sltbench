#pragma once

#include "impl/BenchApi.h"


/*!
	Register function to benchmark

	This benchmark is preferable if:
		* benchmarking function does not require input
		  or input is known at compile time
		* initialization is not required

	\param func - function without arguments

	\example
	\code

		void func()
		{
			// code to benchmark
			// ...
		}
		SLTBENCH_FUNCTION(func);

	\endcode

	\warning
		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION(func) \
	static SLT_DECLARE_DESCR_FUNCTION(func) = SLT_REGISTER_FUNCTION(func);


/*!
	Register function with input set to benchmark

	This benchmark is preferable if:
		* initialization is not required
		* many inputs should be tested
		* input set is known at compile time

	\param func    - function with input argument of type 'const T&'
	\param arg_vec - variable of type 'std::vector<T>'

	\pre 'std::ostream& operator << (std::ostream& os, const T& )' must be defined

	\example
	\code

		void func(const size_t& arg)
		{
			// code to benchmark
			// ...
		}

		static const std::vector<size_t> args{ 128, 256, 512 };

		SLTBENCH_FUNCTION_WITH_ARGS(func, args);

	\endcode

	\warning
		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_ARGS(func, args_vec) \
	SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec); \
	static SLT_DECLARE_DESCR_FUNCTION(func) = SLT_REGISTER_FUNCTION_WITH_ARGS(func, args_vec);


/*!
	Register function with input set generator to benchmark

	This benchmark is preferable if:
		* initialization is not required
		* many inputs should be tested
		* input set is NOT known at compile time
		  (for example, input might be read from file,
		   where filename is given at benchmark
		   command line parameters: argc, argv)

	\param func      - function with input signature 'void(const generator::ArgType&)'
	\param generator - arguments generator class

	\pre generator is default constructible
	\pre generator has inner typedef 'ArgType'
	\pre 'std::ostream& operator << (std::ostream& os, const generator::ArgType& )' must be defined
	\pre generator has member function 'std::vector<ArgType> Generate(int argc, char **argv)'

	\example
	\code

		class Generator
		{
		public:
			typedef std::vector<int> ArgType;

			Generator() {}

			std::vector<ArgType> Generate(int argc, char **argv)
			{
				std::vector<ArgType> rv;
				// ... init args here
				retrun rv;
			}
		};

		std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
		{
			// provide string representation for argument ...
			return os;
		}

		void func(const Generator::ArgType& arg)
		{
			// code to benchmark
			// ...
		}
		SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(func, Generator);

	\endcode

	\warning
		Function name is generated as #func"_"#generator.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(func, generator) \
	SLT_STATIC_ASSERT_IS_GENERATOR(generator); \
	static SLT_DECLARE_DESCR_FUNCTION(func) = \
		SLT_REGISTER_FUNCTION_WITH_ARGS_GENERATOR(func, generator);


/*!
	Register function with lazy generator for input set to benchmark

	This benchmark is preferable if:
		* many inputs should be tested
		* input set is memory-consuming and should be exactly one per each run

	\param func      - function with signature 'void(const generator::ArgType&)'
	\param generator - class of lazy arguments generator

	\pre generator has constructor from int, char**
	\pre generator has inner typedef 'ArgType'
	\pre 'ArgType' must be default-constructible or move-constructible
	\pre 'std::ostream& operator << (std::ostream& os, const generator::ArgType& )' must be defined
	\pre generator has member function 'ArgType Generate()', which generates argument or throws
	     StopGenerationException to stop testing

	\example
	\code

		class Generator
		{
		public:
			typedef MemoryConsumingStruct ArgType;

			Generator(int, char **) {}

			ArgType Generate()
			{
				if (count_ >= 5)
					throw sltbench::StopGenerationException();

				++count_;

				return MemoryConsumingStruct(count_);
			}

		private:
			size_t count_ = 0;
		};

		void func(const Generator::ArgType& arg)
		{
			// code to benchmark
			// ...
		}

		SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, Generator);

	\endcode

	\warning
		Function name is generated as 'func'.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, generator) \
	SLT_STATIC_ASSERT_IS_LAZY_GENERATOR(generator); \
	static SLT_DECLARE_DESCR_FUNCTION(func) = \
		SLT_REGISTER_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, generator);


/*!
	Register function with fixture to benchmark.
	(full version for fixtures)

	This benchmark is preferable if:
		* initialization is required
		* initialization parameters set is known at compile time
		* initialization parameters set is exactly one

	\param func    - function with the only argument of type fixture::Type
	\param fixture - class of fixture

	\pre fixture is default constructible
	\pre fixture has inner typedef Type
	\pre fixture has member function 'Type& Setup()' (which returns _mutable reference_!)
	\pre fixture has member function 'void TearDown()'

	\example
	\code

		class Fixture
		{
		public:
			typedef std::vector<size_t> Type;

			Fixture() {}

			Type& SetUp()
			{
				fixture_.resize(100000, 0);
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void func(Fixture::Type& fixture)
		{
			// code to benchmark
			// ...
		}
		SLTBENCH_FUNCTION_WITH_FIXTURE(func, Fixture);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE(func, fixture) \
	SLT_STATIC_ASSERT_IS_FIXTURE(fixture); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE(func, fixture) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE(func, fixture);


/*!
	Register function with fixture and input set to benchmark

	This benchmark is preferable if:
		* many inputs should be tested
		* input set is known at compile time
		* initialization is required
		* initialization parameters set is exactly one

	\param func    - function with signature 'void(fixture::Type&, const T&)'
	\param fixture - class of fixture
	\param arg_vec - variable of type 'std::vector<T>'

	\pre fixture is default constructible
	\pre fixture has inner typedef Type
	\pre fixture has member function 'Type& Setup(const T&)' (which returns _mutable reference_!)
	\pre fixture has member function 'void TearDown()'
	\pre 'std::ostream& operator << (std::ostream& os, const T& )' must be defined

	\example
	\code

		class Fixture
		{
		public:
			typedef std::vector<size_t> Type;

			Fixture() {}

			Type& SetUp(const size_t& arg)
			{
				fixture_.clear();
				fixture_.reserve(arg);
				for (size_t i = 0; i < arg; ++i)
					fixture_.push_back(i % (arg / 10));
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void func(Fixture::Type& fix, const size_t& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		static const std::vector<size_t> args = { 1024, 2048, 4096 };

		SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(func, Fixture, args);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec) \
	SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec); \
	SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, decltype(args_vec)::value_type); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE(func, fixture) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec);


/*!
	Register function with fixture and input set generator to benchmark

	This benchmark is preferable if:
		* initialization is required
		* many inputs should be tested
		* input set is NOT known at compile time
		  (for example, input might be read from file,
		   where filename is given at benchmark
		   command line parameters: argc, argv)

	\param func      - function with signature 'void(fixture::Type&, const generator::ArgType&)'
	\param fixture   - class of fixture
	\param generator - class of arguments generator

	\pre fixture is default constructible
	\pre fixture has inner typedef Type
	\pre fixture has member function 'Type& Setup(const generator::ArgType&)' (which returns _mutable reference_!)
	\pre fixture has member function 'void TearDown()'
	\pre generator is default constructible
	\pre generator has inner typedef 'ArgType'
	\pre 'std::ostream& operator << (std::ostream& os, const generator::ArgType& )' must be defined
	\pre generator has member function 'std::vector<ArgType> Generate(int argc, char **argv)'

	\example
	\code

		class Generator
		{
		public:
			typedef size_t ArgType;

			Generator() {}

			std::vector<ArgType> Generate(int argc, char **argv)
			{
				std::vector<ArgType> rv;
				// .. init rv
				return rv;
			}
		};

		std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
		{
			// ... some stream representation of the argument
			return os;
		}

		class Fixture
		{
		public:
			typedef std::vector<size_t> Type;

			Fixture() {}

			Type& SetUp(const Generator::ArgType& arg)
			{
				fixture_.clear();
				fixture_.reserve(arg);
				for (size_t i = 0; i < arg; ++i)
					fixture_.push_back(i % (arg / 10));
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void func(Fixture::Type& fix, const Generator::ArgType& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(function, Fixture, Generator);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator) \
	SLT_STATIC_ASSERT_IS_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, generator::ArgType); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE(func, fixture) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator);


/*!
	Register function with fixture and input set lazy generator to benchmark

	This benchmark is preferable if:
		* initialization is required
		* many inputs should be tested
		* input set is memory-consuming and should be exactly one per each run

	\param func      - function with signature 'void(fixture::Type&, const generator::ArgType&)'
	\param fixture   - class of fixture
	\param generator - class of lazy arguments generator

	\pre fixture is default constructible
	\pre fixture has inner typedef Type
	\pre fixture has member function 'Type& Setup(const generator::ArgType&)' (which returns _mutable reference_!)
	\pre fixture has member function 'void TearDown()'
	\pre generator has constructor from int, char**
	\pre generator has inner typedef 'ArgType'
	\pre 'ArgType' must be default-constructible or move-constructible
	\pre 'std::ostream& operator << (std::ostream& os, const generator::ArgType& )' must be defined
	\pre generator has member function 'ArgType Generate()', which generates argument or throws
	     StopGenerationException to stop testing

	\example
	\code

		class Generator
		{
		public:
			typedef size_t ArgType;

			Generator(int, char **) {}

			size_t Generate()
			{
				if (count_ >= 5)
					throw sltbench::StopGenerationException();

				++count_;

				return count_ * 10000;
			}

		private:
			size_t count_ = 0;
		};

		class Fixture
		{
		public:
			typedef std::vector<size_t> Type;

			Fixture() {}

			Type& SetUp(const Generator::ArgType& arg)
			{
				fixture_.clear();
				fixture_.reserve(arg);
				for (size_t i = 0; i < arg; ++i)
					fixture_.push_back(i % (arg / 10));
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void func(Fixture::Type& fix, const Generator::ArgType& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, Fixture, Generator);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator) \
	SLT_STATIC_ASSERT_IS_LAZY_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, generator::ArgType); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE(func, fixture) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator);


/*!
	Register function with fixture to benchmark.
	(simplified version for fixtures: using fixture builder)

	This benchmark is preferable if:
		* initialization is required
		* initialization parameters set is known at compile time
		* initialization parameters set is exactly one
		* fixture supports cheap RAII

	\param func            - function with the only argument
	\param fixture_builder - function constructing the fixtures

	\example
	\code

		std::vector<size_t> make_fixture()
		{
			std::vector<size_t> rv(100000, 0);
			// ... init rv
			return rv;
		}

		void func(std::vector<size_t>& fix)
		{
			// code to benchmark
			// ...
		}
		SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(func, make_fixture);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture_builder.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder);


/*!
	Register function with fixture and input set to benchmark
	(simplified version for fixtures: using fixture builder)

	\example
	\code

		std::vector<size_t> make_fixture(const size_t& arg)
		{
			std::vector<size_t> rv(arg, 0);
			// ... init rv
			return rv;
		}

		void func(std::vector<size_t>& fix, const size_t& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		static const std::vector<size_t> args = { 1024, 2048, 4096 };

		SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(func, make_fixture, args);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture_builder.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(func, fixture_builder, args_vec) \
	SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec); \
	SLT_STATIC_ASSERT_IS_FIXTURE_BUILDER_FOR_ARGS(fixture_builder, decltype(args_vec)::value_type); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(func, fixture_builder, args_vec);


/*!
	Register function with fixture and input set generator to benchmark.
	(simplified version for fixtures: using fixture builder)

	\example
	\code

		class Generator
		{
		public:
			typedef size_t ArgType;

			Generator() {}

			std::vector<ArgType> Generate(int argc, char **argv)
			{
				std::vector<ArgType> rv;
				// ... init rv
				return rv;
			}
		};

		std::vector<size_t> make_fixture(const size_t& arg)
		{
			std::vector<size_t> rv(arg, 0);
			// ... init rv
			return rv;
		}

		void func(std::vector<size_t>& fix, const Generator::ArgType& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(func, make_fixture, Generator);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture_builder"_"generator.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(func, fixture_builder, generator) \
	SLT_STATIC_ASSERT_IS_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_FIXTURE_BUILDER_FOR_ARGS(fixture_builder, generator::ArgType); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(func, fixture_builder, generator);


/*!
	Register function with fixture and input set lazy generator to benchmark.
	(simplified version for fixtures: using fixture builder)

	\example
	\code

		class Generator
		{
		public:
			typedef size_t ArgType;

			Generator(int, char **) {}

			size_t Generate()
			{
				if (count_ >= 5)
					throw sltbench::StopGenerationException();

				++count_;

				return count_ * 10000;
			}

		private:
			size_t count_ = 0;
		};

		std::vector<size_t> make_fixture(const size_t& arg)
		{
			std::vector<size_t> rv(arg, 0);
			// ... init rv
			return rv;
		}

		void func(std::vector<size_t>& fix, const size_t& arg)
		{
			// code to benchmark
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(func, make_fixture, Generator);

	\endcode

	\warning
		Function name is generated as #func"_"#fixture_builder"_"#generator.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(func, fixture_builder, generator) \
	SLT_STATIC_ASSERT_IS_LAZY_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_FIXTURE_BUILDER_FOR_ARGS(fixture_builder, generator::ArgType); \
	static SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) = \
		SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(func, fixture_builder, generator);


/*!
	Entry point

	Defines 'main()' which initializes sltbench and runs benchmarks
*/
#define SLTBENCH_MAIN() \
	int main(int argc, char **argv) \
	{ \
		return ::sltbench::Main(argc, argv); \
	}


//
// customization
//

#define SLTBENCH_CONFIG() \
	SLT_DECLARE_UNIQUE_CONFIG() = ::sltbench::GetConfig()
