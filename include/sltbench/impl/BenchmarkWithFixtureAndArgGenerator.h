#pragma once

#include "Env.h"

#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <vector>


namespace sltbench {

template<typename FixtureT, typename GeneratorT>
class BenchmarkWithFixtureAndArgGenerator
{
public:
	typedef std::function<void(typename FixtureT::Type&, const typename GeneratorT::ArgType&)> FunctionT;

public:
	BenchmarkWithFixtureAndArgGenerator(const char *name, FunctionT function)
		: name_(name)
		, function_(std::move(function))
	{
	}

	BenchmarkWithFixtureAndArgGenerator(const char *name, FunctionT function, std::vector<typename GeneratorT::ArgType> args)
		: name_(name)
		, function_(std::move(function))
		, args_(std::move(args))
	{
	}

public:
	const std::string& GetName() const
	{
		return name_;
	}

	std::chrono::nanoseconds Measure(size_t arg_index)
	{
		const auto& arg = args_[arg_index];

		auto& fix = fixture_->SetUp(arg);

		const auto start_timepoint = std::chrono::high_resolution_clock::now();
		function_(fix, arg);
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

		if (args_.empty())
		{
			const auto& env = Env::Instance();

			args_generator_.reset(new GeneratorT());
			args_ = args_generator_->Generate(env.GetArgc(), env.GetArgv());
		}
	}

	void Finalize()
	{
		fixture_.reset();

		if (args_generator_)
		{
			args_.clear();
			args_.shrink_to_fit();
			args_generator_.reset();
		}
	}

	size_t GetArgsCount()
	{
		return args_.size();
	}

	std::string ConvertArgToString(size_t arg_index)
	{
		std::ostringstream os;
		os << args_[arg_index];
		return os.str();
	}

private:
	std::string name_;
	std::unique_ptr<FixtureT> fixture_;
	FunctionT function_;
	std::unique_ptr<GeneratorT> args_generator_;
	std::vector<typename GeneratorT::ArgType> args_;
};

} // namespace sltbench
