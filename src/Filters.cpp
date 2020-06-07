#include "Filters.h"

#include <cstring>


namespace sltbench {

bool NullFilter::ShouldRunBenchmark(const char*)
{
	return true;
}

RegexFilter::RegexFilter(std::regex re) noexcept
	: re_(std::move(re))
{
}

bool RegexFilter::ShouldRunBenchmark(const char* benchmark_name)
{
	return std::regex_match(benchmark_name,
	                        benchmark_name + std::strlen(benchmark_name),
	                        re_);
}

} // namespace sltbench
