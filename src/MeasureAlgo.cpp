#include "MeasureAlgo.h"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace sltbench {

MeasureAlgo::MeasureAlgo(Conf conf)
	: conf_(std::move(conf))
	, required_spot_size_(0)
	, result_(0)
	, accumulated_execution_time_(std::chrono::nanoseconds::zero())
{
}

void MeasureAlgo::SetFirstTimingResult(std::chrono::nanoseconds time_ns)
{
	for (const auto& dot_param : conf_.dot_params)
	{
		if (time_ns >= dot_param.min_time)
		{
			required_spot_size_ = dot_param.required_spot_size;
			break;
		}
	}

	// fallback to default
	if (required_spot_size_ == 0)
		required_spot_size_ = 26;

	AddTimingResult(time_ns);
}

bool MeasureAlgo::NeedMoreTiming()
{
	const auto results_count = results_ns_.size();

	if (accumulated_execution_time_ > conf_.max_execution_time)
		return false;

	if (accumulated_execution_time_ < conf_.min_execution_time)
		return true;

	if (results_count < required_spot_size_)
		return true;

	auto it = results_ns_.begin();
	for (size_t i = 0; i <= results_count - required_spot_size_; ++i, ++it)
	{
		const auto ref_result = *it;
		bool fits = true;
		auto jt = std::next(it);
		for (size_t j = 1; j < required_spot_size_; ++j, ++jt)
		{
			const auto check_result = *jt;
			if (check_result - ref_result > (ref_result / 100) * conf_.precision_percents)
			{
				fits = false;
				break;
			}
		}
		if (fits)
		{
			result_ = ref_result;
			break;
		}
	}

	return result_ == 0;
}

void MeasureAlgo::AddTimingResult(std::chrono::nanoseconds nanoseconds)
{
	accumulated_execution_time_ += nanoseconds;

	const auto value = nanoseconds.count();
	results_ns_.insert(value);
}

std::chrono::nanoseconds MeasureAlgo::GetResult()
{
	if (result_ != 0)
	{
		// normal case - we found quite stable result!
		return std::chrono::nanoseconds(result_);
	}
	else
	{
		// bad case - timeout or out of retry count
		// then return best result
		if (!results_ns_.empty())
		{
			return std::chrono::nanoseconds(*results_ns_.begin());
		}

		// looks someone wants to see result
		// without timing, well...
		return std::chrono::nanoseconds::zero();
	}
}

} // namespace sltbench
