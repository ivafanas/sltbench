#include "SysInfo.h"

#include <algorithm>
#include <array>


using namespace std::chrono;


namespace {

nanoseconds CalcTimerResolutionAttempt()
{
	const auto start_ts = std::chrono::high_resolution_clock::now();
	while (true)
	{
		const auto final_ts = std::chrono::high_resolution_clock::now();
		if (final_ts > start_ts)
			return final_ts - start_ts;
	}
}

nanoseconds CalcTimerResolution()
{
	std::array<nanoseconds, 4> resolutions;
	for (int i = 0; i < 4; ++i)
		resolutions[i] = CalcTimerResolutionAttempt();
	return *std::min_element(resolutions.begin(), resolutions.end());
}

} // namespace


namespace sltbench {

SysInfo& SysInfo::Instance()
{
	static SysInfo instance;
	return instance;
}

SysInfo::SysInfo()
	: timer_resolution_(CalcTimerResolution())
{
}

std::chrono::nanoseconds SysInfo::GetTimerResolution() const
{
	return timer_resolution_;
}

} // namespace sltbench
