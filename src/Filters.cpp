#include "Filters.h"


namespace sltbench {

bool NullFilter::ShouldRunBenchmark(const std::string&)
{
	return true;
}


RegexFilter::RegexFilter(std::regex re)
	: re_(std::move(re))
{
}

bool RegexFilter::ShouldRunBenchmark(const std::string& bench_name)
{
	return std::regex_match(bench_name.begin(), bench_name.end(), re_);
}

} // namespace sltbench
