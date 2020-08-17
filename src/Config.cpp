#include "Config.h"

#include "MeasureAlgo.h"
#include "Reporters.h"


namespace sltbench {

Config::Config()
{
	measure_conf.precision_percents = 5;
	measure_conf.max_execution_time = std::chrono::minutes(1);
	measure_conf.min_execution_time = std::chrono::milliseconds(200);
	measure_conf.dot_params = {
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

Config::~Config() noexcept = default;

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

} // namespace sltbench
