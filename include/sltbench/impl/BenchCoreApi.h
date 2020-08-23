#pragma once

#include "DoNotOptimize.h"
#include "Descriptor.h"


namespace sltbench {

typedef void(*SLTFun)();


//! calls Init and Run
int Main(int argc, char **argv);


//! initialize benchmark from command line options
void Init(int argc, char **argv);


//! run benchmarking tool
int Run();


/*!
	Register function for benchmarking

	\param name - function name
	\param func - function for benchmarking
*/
Descriptor *RegisterBenchmark(const char *name, SLTFun func);

} // namespace sltbench


//
// define internal helpers
//

#define SLT_CONCATENATE_2_IMPL(s1, s2) s1##s2
#define SLT_CONCATENATE_2(s1, s2) SLT_CONCATENATE_2_IMPL(s1, s2)

#define SLT_DECLARE_DESCR_FUNCTION(func) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc_##func,__LINE__)
#define SLT_REGISTER_FUNCTION(func) ::sltbench::RegisterBenchmark(#func, func)

