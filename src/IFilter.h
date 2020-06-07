#pragma once


namespace sltbench {

struct IFilter
{
	virtual ~IFilter() noexcept = default;

	virtual bool ShouldRunBenchmark(const char* benchmark_name) = 0;
};

} // namespace sltbench
