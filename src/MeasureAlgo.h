#pragma once

#include <sltbench/impl/IMeasureAlgo.h>

#include <memory>
#include <vector>


namespace sltbench {

class MAResultsContainer;

class MeasureAlgo final : public IMeasureAlgo
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

public: // IMeasureAlgo
	virtual void SetFirstTimingResult(std::chrono::nanoseconds nanoseconds) override;
	virtual bool NeedMoreTiming() override;
	virtual void AddTimingResult(std::chrono::nanoseconds nanoseconds) override;
	virtual std::chrono::nanoseconds GetResult() override;

private:
	Conf conf_;
	size_t required_spot_size_;
	std::chrono::nanoseconds accumulated_execution_time_;
	uint64_t result_;
	std::unique_ptr<MAResultsContainer> results_container_;
};

} // namespace sltbench
