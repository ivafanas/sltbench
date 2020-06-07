#pragma once

#include <regex>


namespace sltbench {

class RegexFilter
{
public:
	RegexFilter(std::regex re) noexcept;
	~RegexFilter() noexcept = default;

	bool ShouldRunBenchmark(const char* benchmark_name);

private:
	std::regex re_;
};

} // namespace sltbench
