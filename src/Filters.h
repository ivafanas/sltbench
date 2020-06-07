#pragma once

#include "IFilter.h"

#include <regex>


namespace sltbench {

class RegexFilter final
	: public IFilter
{
public:
	RegexFilter(std::regex re) noexcept;
	~RegexFilter() noexcept override = default;

public: // IFilter
	bool ShouldRunBenchmark(const char* benchmark_name) override;

private:
	std::regex re_;
};

} // namespace sltbench
