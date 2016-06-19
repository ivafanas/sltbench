#pragma once

#include "IMeasureAlgo.h"
#include "IReporter.h"

#include <chrono>
#include <functional>
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
	Private config interface for autolearning,
	user should not use it
*/
struct IConfigPrivate
{
	virtual ~IConfigPrivate() {}

	virtual std::unique_ptr<IMeasureAlgo> CreateMeasureAlgo() = 0;
	virtual bool IsMeasureRequired(std::chrono::nanoseconds estimation_time) = 0;
};


//! access to user configurations
IConfig& GetConfig();

} // namespace sltbench
