#pragma once

#include <chrono>
#include <string>


namespace sltbench {

template<typename FixtureT>
class Benchmark_FB
{
public:
	typedef void(*FunctionT)(FixtureT&);
	typedef FixtureT(*FixtureBuilderT)();

public:
	Benchmark_FB(const char *name, FunctionT function, FixtureBuilderT builder)
		: name_(name)
		, function_(function)
		, builder_(builder)
	{
	}

public:
	std::chrono::nanoseconds Measure(size_t)
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

	const std::string& GetName() const { return name_; }
	bool SupportsMulticall() const { return false; }
	void Prepare() { measured_ = false; }
	void Finalize() {}
	bool HasArgsToProcess() { return !measured_; }
	void OnArgProcessed() { measured_ = true; }
	std::string CurrentArgAsString() { return{}; }

private:
	std::string name_;
	FunctionT function_;
	FixtureBuilderT builder_;
	bool measured_ = false;
};

} // namespace sltbench
