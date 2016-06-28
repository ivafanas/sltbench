#pragma once

#include <sltbench/impl/IConfig.h>
#include <sltbench/impl/IMeasureAlgo.h>

#include "MeasureAlgo.h"

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
	virtual IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) override;
	virtual IConfigPrivate& GetPrivate() override;

public: // IConfigPrivate
	virtual std::unique_ptr<IMeasureAlgo> CreateMeasureAlgo() override;
	virtual bool IsMeasureRequired(std::chrono::nanoseconds estimation_time) override;
	virtual IFilter& GetFilter() override;
	virtual void SetFilter(std::unique_ptr<IFilter> filter) override;

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
};

} // namespace sltbench
