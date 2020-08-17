#include "MeasureAlgo.h"

#include "MAResultsContainer.h"


using namespace std::chrono;


namespace sltbench {

static size_t CalcRequiredSpotSize(
	const MeasureAlgo::Conf& conf,
	nanoseconds first_res) noexcept
{
	size_t rv = 0;
	for (const auto& dot_param : conf.dot_params)
	{
		if (first_res >= dot_param.min_time)
		{
			rv = dot_param.required_spot_size;
			break;
		}
	}
	const size_t c_default_spot_size = 26;
	return !rv ? c_default_spot_size : rv;
}

MeasureAlgo::MeasureAlgo(Conf conf) noexcept
	: conf_(std::move(conf))
	, required_spot_size_(0)
	, accumulated_execution_time_(nanoseconds::zero())
	, result_(0)
{
}

MeasureAlgo::~MeasureAlgo() noexcept = default;

void MeasureAlgo::SetEstimationResult(const EstimationResult& estimation)
{
	required_spot_size_ = CalcRequiredSpotSize(conf_, estimation.result);

	results_container_.reset(new MAResultsContainer);

	SingleMeasureResult mr;
	mr.result = estimation.result;
	mr.total_time = estimation.total_time;
	AddTimingResult(mr);
}

bool MeasureAlgo::NeedMoreTiming() noexcept
{
	if (accumulated_execution_time_ > conf_.max_execution_time)
		return false;

	if (accumulated_execution_time_ < conf_.min_execution_time)
		return true;

	result_ = results_container_->GetMinSpotValue(
		required_spot_size_,
		conf_.precision_percents);

	return result_ == 0;
}

void MeasureAlgo::AddTimingResult(const SingleMeasureResult& result)
{
	if (result.result.count() > 0)
	{
		accumulated_execution_time_ += result.total_time;

		results_container_->Add(result.result.count());
	}
}

nanoseconds MeasureAlgo::GetResult() noexcept
{
	if (result_ != 0)
	{
		// normal case - we found quite stable result!
		return nanoseconds(result_);
	}
	else
	{
		// bad case - timeout or out of retry count
		// then return best result
		if (results_container_)
			return nanoseconds(results_container_->GetBest());

		// looks someone wants to see result
		// without timing, well...
		return nanoseconds::zero();
	}
}

} // namespace sltbench
