#pragma once

#include <sltbench/impl/IConfig.h>
#include <sltbench/impl/Optional.h>

#include "Filters.h"
#include "MeasureAlgo.h"

#include <memory>


namespace sltbench {

class Config final
	: public IConfig
{
public:
	static Config& Instance();

public: // IConfig
	IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) noexcept override;

public:
	scoped_optional<RegexFilter> filter;
	MeasureAlgo::Conf measure_conf;
	bool is_heatup_required = true;

	reporter::IReporter& GetReporter();

private:
	Config();
	~Config() noexcept override;

	std::unique_ptr<reporter::IReporter> reporter_;
	MeasureAlgo::Conf measure_conf_;
};

} // namespace sltbench
