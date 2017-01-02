#include "Config.h"

#include "ConsoleReporter.h"
#include "Filters.h"
#include "MeasureAlgo.h"


namespace sltbench {

Config::Config()
	: is_measure_required_pred_([](std::chrono::nanoseconds ns) { return true; })
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

IConfig& Config::SetReporter(std::unique_ptr<reporter::IReporter> reporter)
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

IConfigPrivate& Config::GetPrivate()
{
	return *this;
}

void Config::SetMeasureAlgoConf(MeasureAlgo::Conf conf)
{
	measure_conf_ = std::move(conf);
}

std::unique_ptr<IMeasureAlgo> Config::CreateMeasureAlgo()
{
	return std::unique_ptr<MeasureAlgo>(new MeasureAlgo(measure_conf_));
}

bool Config::IsMeasureRequired(std::chrono::nanoseconds estimation_time)
{
	return is_measure_required_pred_(estimation_time);
}

void Config::SetMeasureRequiredPred(std::function<bool(std::chrono::nanoseconds)> pred)
{
	is_measure_required_pred_ = std::move(pred);
}

IFilter& Config::GetFilter()
{
	if (!filter_)
	{
		filter_.reset(new NullFilter());
	}
	return *filter_;
}

void Config::SetFilter(std::unique_ptr<IFilter> filter)
{
	filter_ = std::move(filter);
}

void Config::SetHeatupRequired(bool heatup_required)
{
	is_heatup_required_ = heatup_required;
}

bool Config::IsHeatupRequired()
{
	return is_heatup_required_;
}

} // namespace sltbench
