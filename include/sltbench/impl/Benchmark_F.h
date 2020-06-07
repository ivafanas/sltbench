#pragma once

#include "IBenchmark.h"
#include "Optional.h"

#include <chrono>
#include <string>


namespace sltbench {

template<typename FixtureT>
class Benchmark_F : public IBenchmark
{
public:
	typedef void(*FunctionT)(typename FixtureT::Type&);

public:
	Benchmark_F(const char *name, FunctionT function)
		: IBenchmark(name, /*supports_multicall*/ false)
		, function_(function)
	{
	}

public:
	std::chrono::nanoseconds Measure(size_t) override
	{
		auto& fix = fixture_opt_.get().SetUp();

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fix);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		const auto rv =
			final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);

		fixture_opt_.get().TearDown();

		return rv;
	}

	void Prepare() override
	{
		fixture_opt_.emplace();
		measured_ = false;
	}

	void Finalize() override
	{
		fixture_opt_.reset();
	}

	bool HasArgsToProcess() override
	{
		return !measured_;
	}

	void OnArgProcessed() override
	{
		measured_ = true;
	}

	std::string CurrentArgAsString() override
	{
		return{};
	}

private:
	scoped_optional<FixtureT> fixture_opt_;
	FunctionT function_;
	bool measured_ = false;
};

} // namespace sltbench
