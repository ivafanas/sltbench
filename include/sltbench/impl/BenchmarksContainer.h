#pragma once


namespace sltbench {

class IBenchmark;

// Accept ownership of |benchmark| and save it to a common set of benchmarks.
//
// NOTE: |benchmark| is an OWNING raw pointer!!
//       It is for compile-time and binary size optimization.
void RegisterBenchmark(IBenchmark* benchmark);

} // namespace sltbench
