#pragma once

#include <string>


namespace sltbench {

struct IFilter
{
	virtual ~IFilter() = default;

	virtual bool ShouldRunBenchmark(const std::string& benchmark_name) = 0;
};

} // namespace sltbench
