#pragma once

#include "impl/BenchCoreApi.h"


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
    Entry point

    Defines 'main()' which initializes sltbench and runs benchmarks
*/
#define SLTBENCH_MAIN() \
    int main(int argc, char **argv) \
    { \
        return ::sltbench::Main(argc, argv); \
    }

