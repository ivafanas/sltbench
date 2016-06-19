#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>



namespace sltbench {

template<typename FixtureT>
class BenchmarkWithFixture
{
public:
	typedef std::function<void(typename FixtureT::Type&)> FunctionT;

public:
	BenchmarkWithFixture(const char *name, FunctionT function)
		: name_(name)
		, function_(std::move(function))
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

		const auto start_timepoint = std::chrono::high_resolution_clock::now();
		function_(fix);
		const auto final_timepoint = std::chrono::high_resolution_clock::now();
		const auto rv = 
			final_timepoint > start_timepoint
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_timepoint - start_timepoint)
			: std::chrono::nanoseconds(0);

		fixture_->TearDown();

		return rv;
	}

	void Prepare()
	{
		fixture_.reset(new FixtureT());
	}

	void Finalize()
	{
		fixture_.reset();
	}

	size_t GetArgsCount()
	{
		return 1;
	}

	std::string ConvertArgToString(size_t)
	{
		return{};
	}

private:
	std::string name_;
	std::unique_ptr<FixtureT> fixture_;
	FunctionT function_;
};

} // namespace sltbench
