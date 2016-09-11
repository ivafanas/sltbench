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

		void my_functions()
		{
			// some stuff here to benchmark
			...
		}
		SLTBENCH_FUNCTION(my_functions);

	\endcode

	\warning
		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION(func) \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) = SLT_REGISTER_FUNCTION(func);


/*!
	Register function with fixture to benchmark

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

		void my_function(MyFixture::Type& fixture)
		{
			// do some stuff with fixture here to benchmark
			...
		}
		SLTBENCH_FUNCTION_WITH_FIXTURE(my_function, MyFixture);

	\endcode

	\warning
		Function name is generated as func##fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE(func, fixture) \
	SLT_STATIC_ASSERT_IS_FIXTURE(fixture); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) = SLT_REGISTER_FUNCTION_WITH_FIXTURE(func, fixture);


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

		void my_function(const size_t& arg)
		{
			// do some stuff with arg here to benchmark
			...
		}

		static const std::vector<size_t> my_args{ 32, 64, 128, 256, 512, 42 };

		SLTBENCH_FUNCTION_WITH_ARGS(my_function, my_args);

	\endcode

	\warning
		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_ARGS(func, args_vec) \
	SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) = SLT_REGISTER_FUNCTION_WITH_ARGS(func, args_vec);


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
			// ... provide string representation for argument
			return os;
		}

		void my_function(const Generator::ArgType& arg)
		{
			// some stuff with arg here to benchmark
		}
		SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(my_function, Generator);

	\endcode

	\warning
		Function name is generated as func##generator.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(func, generator) \
	SLT_STATIC_ASSERT_IS_GENERATOR(generator); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) = SLT_REGISTER_FUNCTION_WITH_ARGS_GENERATOR(func, generator);


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

        void my_function(const Generator::ArgType& arg)
        {
            // some useful work here ...
        }

        SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(my_function, Generator);

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
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) = SLT_REGISTER_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, generator);


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

		class MyFixture
		{
		public:
			typedef std::vector<size_t> Type;

			MyFixture() {}

			Type& SetUp(const size_t& arg)
			{
				fixture_.clear();
				fixture_.reserve(arg);
				for (size_t i = 0; i < arg; ++i)
				{
					fixture_.push_back(i % (arg / 10));
				}
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void my_function(MyFixture::Type& fix, const size_t& arg)
		{
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		static const std::vector<size_t> my_args = { 1024 * 1024, 42 * 1024 * 1024 };

		SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(my_function, MyFixture, size_t);

	\endcode

	\warning
		Function name is generated as func##fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec) \
	SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec); \
    SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, decltype(args_vec)::value_type); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) = SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec);


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

		class MyArgsGenerator
		{
		public:
			typedef size_t ArgType;

			MyArgsGenerator() {}

			std::vector<ArgType> Generate(int argc, char **argv)
			{
				std::vector<ArgType> rv;
				// .. init rv
				return rv;
			}
		};

		std::ostream& operator << (std::ostream& os, const MyArgsGenerator::ArgType& rhs)
		{
			// ...
			return os;
		}

		class MyFixture
		{
		public:
			typedef std::vector<size_t> Type;

			MyFixture() {}

			Type& SetUp(const MyArgsGenerator::ArgType& arg)
			{
				fixture_.clear();
				fixture_.reserve(arg);
				for (size_t i = 0; i < arg; ++i)
				{
					fixture_.push_back(i % (arg / 10));
				}
				return fixture_;
			}

			void TearDown() {}

		private:
			Type fixture_;
		};

		void my_function(MyFixture::Type& fix, const MyArgsGenerator::ArgType& arg)
		{
			assert(fix.size() == arg);
			std::sort(fix.begin(), fix.end());
		}

		SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(my_function, MyFixture, MyArgsGenerator);


	\endcode

	\warning
		Function name is generated as func##fixture.

		Further functions registered with the same name
		even in other source files will be ignored.
		sltbench ensures unique name for functions to
		guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator) \
    SLT_STATIC_ASSERT_IS_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, generator::ArgType); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) = SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator);


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

        void my_function(Fixture::Type& fix, const Generator::ArgType& arg)
        {
            assert(fix.size() == arg);
            std::sort(fix.begin(), fix.end());
        }

        SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(my_function, Fixture, Generator);


    \endcode

    \warning
        Function name is generated as func##fixture.

        Further functions registered with the same name
        even in other source files will be ignored.
        sltbench ensures unique name for functions to
        guarantee the same functions order per run.
*/
#define SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator) \
    SLT_STATIC_ASSERT_IS_LAZY_GENERATOR(generator); \
	SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, generator::ArgType); \
	static SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) = SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator);


/*!
	Entry point

	Defines 'main()' wich initializes sltbench and runs benchmarks
*/
#define SLTBENCH_MAIN() \
	int main(int argc, char **argv) \
	{ \
		::sltbench::Init(argc, argv); \
		return ::sltbench::Run(); \
	}


//
// customization
//

#define SLTBENCH_CONFIG() \
	SLT_DECLARE_UNIQUE_CONFIG() = ::sltbench::GetConfig()
