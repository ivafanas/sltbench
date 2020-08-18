#include "SysInfo.h"

#include <algorithm>


using namespace std::chrono;


static nanoseconds CalcTimerResolutionAttempt() noexcept
{
	const auto start_ts = std::chrono::high_resolution_clock::now();
	while (true)
	{
		const auto final_ts = std::chrono::high_resolution_clock::now();
		if (final_ts > start_ts)
			return final_ts - start_ts;
	}
}

static nanoseconds CalcTimerResolution() noexcept
{
	const nanoseconds resolutions[4] = {
		CalcTimerResolutionAttempt(),
		CalcTimerResolutionAttempt(),
		CalcTimerResolutionAttempt(),
		CalcTimerResolutionAttempt()
	};
	return *std::min_element(resolutions, resolutions + 4);
}


namespace sltbench {

SysInfo& SysInfo::Instance()
{
	static SysInfo instance;
	return instance;
}

SysInfo::SysInfo() noexcept
	: timer_resolution_(CalcTimerResolution())
{
}

std::chrono::nanoseconds SysInfo::GetTimerResolution() const noexcept
{
	return timer_resolution_;
}

} // namespace sltbench
