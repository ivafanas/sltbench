#pragma once

#include <vector>


namespace sltbench {

class IBenchmark;

// return registered benchmarks sorted by name
std::vector<IBenchmark*> GetRegisteredBenchmarks();

}  // namespace sltbench

