#pragma once

#include "SingleMeasureAlgo.h"

#include <memory>
#include <vector>


namespace sltbench {

class MAResultsContainer;

class MeasureAlgo
{
public:
	struct Conf
	{
		// DOT - Dependent On estimation Time
		struct DOTParam
		{
			std::chrono::nanoseconds min_time;
			size_t required_spot_size;
		};
		std::vector<DOTParam> dot_params;

		std::chrono::nanoseconds max_execution_time;
		std::chrono::nanoseconds min_execution_time;

		uint8_t precision_percents;
	};

public:
	MeasureAlgo(Conf conf) noexcept;
	~MeasureAlgo() noexcept;

	void SetEstimationResult(const EstimationResult& estimation);
	bool NeedMoreTiming() noexcept;
	void AddTimingResult(const SingleMeasureResult& result);
	std::chrono::nanoseconds GetResult() noexcept;

private:
	Conf conf_;
	size_t required_spot_size_;
	std::chrono::nanoseconds accumulated_execution_time_;
	uint64_t result_;
	std::unique_ptr<MAResultsContainer> results_container_;
};

} // namespace sltbench
