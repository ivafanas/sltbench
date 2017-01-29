#pragma once

#include "SingleMeasureAlgo.h"

#include <chrono>


namespace sltbench {

/*!
	Interface for measurement algorithm

	Usage:
	\code

		// make the first timing result and
		// set it to IMeasureAlgo to evaluate
		// best-fit measure sub-algorithm
		auto timing_res_1 = ...;
		measure_algo->SetFirstTimingResult(timing_res_1);

		// add timing results
		// while measure is not good enough
		while (measure_algo->NeedMoreTiming())
		{
			auto timing_res = ...;
			measure_algo->AddTimingResult(...);
		}

		// get the final measurement result!
		auto measure_res = measure_algo->GetResult();

	\endcode
*/
struct IMeasureAlgo
{
	virtual ~IMeasureAlgo() = default;

	//! set the first timing result
	virtual void SetEstimationResult(const single_measure_algo::EstimationResult& estimation) = 0;

	//! whether one more timing is needed for measurement
	virtual bool NeedMoreTiming() = 0;

	//! add timing result
	virtual void AddTimingResult(const single_measure_algo::SingleMeasureResult& result) = 0;

	//! get measurement result
	virtual std::chrono::nanoseconds GetResult() = 0;
};

} // namespace sltbench
