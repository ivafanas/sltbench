#pragma once

#include <sltbench/impl/IFilter.h>

#include <regex>


namespace sltbench {

class NullFilter final
	: public IFilter
{
public:
	NullFilter() = default;
	virtual ~NullFilter() override = default;

public: // IFilter
	virtual bool ShouldRunBenchmark(const std::string& benchmark_name) override;
};


class RegexFilter final
	: public IFilter
{
public:
	RegexFilter(std::regex re);
	virtual ~RegexFilter() override = default;

public: // IFilter
	virtual bool ShouldRunBenchmark(const std::string& benchmark_name) override;

private:
	std::regex re_;
};

} // namespace sltbench
