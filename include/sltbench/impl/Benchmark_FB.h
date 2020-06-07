#pragma once

#include "IBenchmark.h"

#include <chrono>
#include <string>


namespace sltbench {

template<typename FixtureT>
class Benchmark_FB : public IBenchmark
{
public:
	typedef void(*FunctionT)(FixtureT&);
	typedef FixtureT(*FixtureBuilderT)();

public:
	Benchmark_FB(const char *name, FunctionT function, FixtureBuilderT builder)
		: IBenchmark(name, /*supports_multicall*/ false)
		, function_(function)
		, builder_(builder)
	{
	}

public:
	std::chrono::nanoseconds Measure(size_t) override
	{
		auto fixture = builder_();

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fixture);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		const auto rv =
			final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);

		return rv;
	}

	void Prepare() override { measured_ = false; }
	void Finalize() override {}
	bool HasArgsToProcess() override { return !measured_; }
	void OnArgProcessed() override { measured_ = true; }
	std::string CurrentArgAsString() override { return{}; }

private:
	FunctionT function_;
	FixtureBuilderT builder_;
	bool measured_ = false;
};

} // namespace sltbench
