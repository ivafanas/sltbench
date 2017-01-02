#pragma once

#include <sltbench/impl/IConfig.h>
#include <sltbench/impl/IMeasureAlgo.h>

#include "MeasureAlgo.h"

#include <functional>
#include <memory>


namespace sltbench {

class Config final
	: public IConfig
	, public IConfigPrivate
{
public:
	static Config& Instance();

private:
	Config();
	virtual ~Config() override;

public: // IConfig
	IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) override;
	IConfigPrivate& GetPrivate() override;

public: // IConfigPrivate
	std::unique_ptr<IMeasureAlgo> CreateMeasureAlgo() override;
	bool IsMeasureRequired(std::chrono::nanoseconds estimation_time) override;
	IFilter& GetFilter() override;
	void SetFilter(std::unique_ptr<IFilter> filter) override;
	void SetHeatupRequired(bool heatup_required) override;
	bool IsHeatupRequired() override;


public:
	void SetMeasureAlgoConf(MeasureAlgo::Conf conf);
	void SetMeasureRequiredPred(std::function<bool(std::chrono::nanoseconds)> pred);

public:
	reporter::IReporter& GetReporter();

private:
	std::unique_ptr<reporter::IReporter> reporter_;
	std::unique_ptr<IFilter> filter_;
	MeasureAlgo::Conf measure_conf_;
	std::function<bool(std::chrono::nanoseconds)> is_measure_required_pred_;
	bool is_heatup_required_ = true;
};

} // namespace sltbench
