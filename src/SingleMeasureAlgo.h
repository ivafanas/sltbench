#pragma once

#include <chrono>
#include <functional>


namespace sltbench {
namespace single_measure_algo {

using SingleMeasureFun = std::function<std::chrono::nanoseconds(size_t)>;

//! result of estimation algorithm
struct EstimationResult
{
	//! function execution time
	std::chrono::nanoseconds result;

	//! total estimation time
	std::chrono::nanoseconds total_time;

	//! recommended calls count per measure
	size_t recommended_calls_count = 1;

	//! important note about timing
	enum class Verdict
	{
		OK,
		CANNOT_BE_PRECISE,
	};
	Verdict verdict = Verdict::OK;
};

//! estimate execution time
EstimationResult Estimate(const SingleMeasureFun& measure_func, bool enable_multicall);


//! result of measure function
struct SingleMeasureResult
{
	//! time per call
	std::chrono::nanoseconds result;

	//! total function testing time
	std::chrono::nanoseconds total_time;
};

//! make measure
std::chrono::nanoseconds Measure(
	const SingleMeasureFun& measure_func,
	const EstimationResult& estimation);

} // namespace single_measure_algo
} // namespace sltbench
