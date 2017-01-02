#pragma once

#include "IFilter.h"
#include "IMeasureAlgo.h"
#include "IReporter.h"

#include <chrono>
#include <memory>


namespace sltbench {


struct IConfigPrivate;


//! config interface, visible to user
struct IConfig
{
	virtual ~IConfig() {}

	//! set custom reporter
	virtual IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) = 0;

	//! access to private config functionality (user shouldn't do this)
	virtual IConfigPrivate& GetPrivate() = 0;
};


/*!
	Private config interface,
	user should not use it
*/
struct IConfigPrivate
{
	virtual ~IConfigPrivate() {}

	virtual std::unique_ptr<IMeasureAlgo> CreateMeasureAlgo() = 0;
	virtual bool IsMeasureRequired(std::chrono::nanoseconds estimation_time) = 0;

	virtual IFilter& GetFilter() = 0;
	virtual void SetFilter(std::unique_ptr<IFilter> filter) = 0;

	virtual void SetHeatupRequired(bool heatup_required) = 0;
	virtual bool IsHeatupRequired() = 0;
};


//! access to user configurations
IConfig& GetConfig();

} // namespace sltbench
