#pragma once

#include <chrono>


namespace sltbench {

class SysInfo final
{
public:
	static SysInfo& Instance();

private:
	SysInfo() noexcept;

public:
	std::chrono::nanoseconds GetTimerResolution() const noexcept;

private:
	std::chrono::nanoseconds timer_resolution_;
};

} // namespace sltbench
