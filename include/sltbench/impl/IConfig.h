#pragma once

#include "IReporter.h"

#include <memory>


namespace sltbench {

//! config interface, visible to user
struct IConfig
{
	virtual ~IConfig() noexcept = default;

	//! set custom reporter
	virtual IConfig& SetReporter(std::unique_ptr<reporter::IReporter> reporter) noexcept = 0;
};

//! access to user configurations
IConfig& GetConfig();

} // namespace sltbench
