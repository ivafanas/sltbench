#pragma once

#include <sltbench/impl/IConfig.h>
#include <sltbench/impl/IFilter.h>
#include <sltbench/impl/IMeasureAlgo.h>

#include "MeasureAlgo.h"

#include <memory>


namespace sltbench {

class Config final
	: public IConfig
{
public:
	static Config& Instance();

public: // IConfig
	IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) override;

public:
	std::unique_ptr<IMeasureAlgo> CreateMeasureAlgo();
	IFilter& GetFilter();
	void SetFilter(std::unique_ptr<IFilter> filter);
	void SetHeatupRequired(bool heatup_required);
	bool IsHeatupRequired();
	void SetMeasureAlgoConf(MeasureAlgo::Conf conf);
	reporter::IReporter& GetReporter();

private:
	Config();
	~Config() override;

	std::unique_ptr<reporter::IReporter> reporter_;
	std::unique_ptr<IFilter> filter_;
	MeasureAlgo::Conf measure_conf_;
	bool is_heatup_required_ = true;
};

} // namespace sltbench
