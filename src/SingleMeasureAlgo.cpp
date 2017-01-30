#include <sltbench/impl/IConfig.h>
#include <sltbench/impl/SingleMeasureAlgo.h>

#include "SysInfo.h"

#include <algorithm>


using namespace std::chrono;


namespace sltbench {
namespace single_measure_algo {
namespace {

nanoseconds GetMinExecutionTimeForPreciseResult()
{
	// timer error should be at max 0.5% of function execution time
	return SysInfo::Instance().GetTimerResolution() * 200;
}

} // namespace


EstimationResult Estimate(const SingleMeasureFun& measure_func, const bool enable_multicall)
{
	// estimaton should be at least 10 ms
	const milliseconds min_estimation_time(10);

	// min function execution time, that timer would not influe on result error
	const nanoseconds min_execution_time = GetMinExecutionTimeForPreciseResult();

	// make the first run
	nanoseconds total_time = measure_func(1u);

	// If function execution time is very small, that timer precision signigicantly influe at
	// measure result, we may try to produce reasonable performance measure results if and only if
	// benchmark supports multicalls.
	// Otherwise performance cannot be measured.
	const EstimationResult::Verdict verdict =
		total_time > min_execution_time || enable_multicall
		? EstimationResult::Verdict::OK
		: EstimationResult::Verdict::CANNOT_BE_PRECISE;

	size_t total_calls = 1;
	size_t calls_per_measure = 1;
	while (total_time < min_estimation_time)
	{
		if (enable_multicall)
			calls_per_measure *= 2;
		total_time += measure_func(calls_per_measure);
		total_calls += calls_per_measure;
	}

	EstimationResult rv;
	rv.total_time = total_time;
	rv.result = std::max<nanoseconds>(nanoseconds(1), total_time / total_calls);
	rv.recommended_calls_count = enable_multicall
		? std::max<size_t>(1u, static_cast<size_t>(min_execution_time.count() / rv.result.count()))
		: 1u;
	rv.verdict = verdict;
	return rv;
}

nanoseconds Measure(const SingleMeasureFun& measure_func, const EstimationResult& estimation)
{
	const auto calls_count = estimation.recommended_calls_count;

	const auto measure_algo = GetConfig().GetPrivate().CreateMeasureAlgo();
	measure_algo->SetEstimationResult(estimation);
	while (measure_algo->NeedMoreTiming())
	{
		const auto time_ns = measure_func(calls_count);

		SingleMeasureResult mr;
		mr.total_time = time_ns;
		mr.result = std::max<nanoseconds>(nanoseconds(1), time_ns / calls_count);
		measure_algo->AddTimingResult(mr);
	}
	return measure_algo->GetResult();
}

} // namespace single_measure_algo
} // namespace sltbench
