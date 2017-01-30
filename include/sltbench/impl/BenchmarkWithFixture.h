#pragma once

#include <chrono>
#include <memory>
#include <string>


namespace sltbench {

template<typename FixtureT>
class BenchmarkWithFixture
{
public:
	typedef void(*FunctionT)(typename FixtureT::Type&);

public:
	BenchmarkWithFixture(const char *name, FunctionT function)
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
		auto& fix = fixture_->SetUp();

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fix);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		const auto rv =
			final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);

		fixture_->TearDown();

		return rv;
	}

	bool SupportsMulticall() const
	{
		return false;
	}

	void Prepare()
	{
		fixture_.reset(new FixtureT());
		measured_ = false;
	}

	void Finalize()
	{
		fixture_.reset();
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
	std::unique_ptr<FixtureT> fixture_;
	FunctionT function_;
	bool measured_ = false;
};

} // namespace sltbench
