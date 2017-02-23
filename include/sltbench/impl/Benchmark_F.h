#pragma once

#include "Optional.h"

#include <chrono>
#include <string>


namespace sltbench {

template<typename FixtureT>
class Benchmark_F
{
public:
	typedef void(*FunctionT)(typename FixtureT::Type&);

public:
	Benchmark_F(const char *name, FunctionT function)
		: name_(name)
		, function_(function)
	{
	}

public:
	const std::string& GetName() const
	{
		return name_;
	}

	std::chrono::nanoseconds Measure(size_t)
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

	bool SupportsMulticall() const
	{
		return false;
	}

	void Prepare()
	{
		fixture_opt_.emplace();
		measured_ = false;
	}

	void Finalize()
	{
		fixture_opt_.reset();
	}

	bool HasArgsToProcess()
	{
		return !measured_;
	}

	void OnArgProcessed()
	{
		measured_ = true;
	}

	std::string CurrentArgAsString()
	{
		return{};
	}

private:
	std::string name_;
	scoped_optional<FixtureT> fixture_opt_;
	FunctionT function_;
	bool measured_ = false;
};

} // namespace sltbench
