#include "Config.h"

#include "Filters.h"
#include "MeasureAlgo.h"
#include "Reporters.h"


namespace sltbench {

Config::Config()
{
	measure_conf_.precision_percents = 5;
	measure_conf_.max_execution_time = std::chrono::minutes(1);
	measure_conf_.min_execution_time = std::chrono::milliseconds(200);
	measure_conf_.dot_params = {
		{ std::chrono::seconds(3), 1 },
		{ std::chrono::milliseconds(100), 3 },
		{ std::chrono::milliseconds(50),  4 },
		{ std::chrono::milliseconds(20),  5 },
		{ std::chrono::milliseconds(10),  10 },
		{ std::chrono::microseconds(250), 15 },
		{ std::chrono::microseconds(100), 18 },
		{ std::chrono::nanoseconds::zero(), 20 }
	};
}

Config::~Config()
{
}

Config& Config::Instance()
{
	static Config config;
	return config;
}

IConfig& Config::SetReporter(std::unique_ptr<reporter::IReporter> reporter) noexcept
{
	reporter_ = std::move(reporter);
	return *this;
}

reporter::IReporter& Config::GetReporter()
{
	if (!reporter_)
	{
		// fallback to default
		reporter_.reset(new reporter::ConsoleReporter());
	}

	return *reporter_;
}

void Config::SetMeasureAlgoConf(MeasureAlgo::Conf conf) noexcept
{
	measure_conf_ = std::move(conf);
}

std::unique_ptr<IMeasureAlgo> Config::CreateMeasureAlgo()
{
	return std::unique_ptr<MeasureAlgo>(new MeasureAlgo(measure_conf_));
}

IFilter& Config::GetFilter()
{
	if (!filter_)
	{
		filter_.reset(new NullFilter());
	}
	return *filter_;
}

void Config::SetFilter(std::unique_ptr<IFilter> filter) noexcept
{
	filter_ = std::move(filter);
}

} // namespace sltbench
