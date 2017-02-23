#pragma once

#include "Env.h"
#include "Optional.h"

#include <chrono>
#include <sstream>
#include <string>
#include <vector>


namespace sltbench {

template<typename FixtureT, typename GeneratorT>
class Benchmark_F_AG
{
public:
	typedef typename FixtureT::Type FixT;
	typedef typename GeneratorT::ArgType ArgT;
	typedef void (*FunctionT)(FixT&, const ArgT&);

public:
	Benchmark_F_AG(const char *name, FunctionT function)
		: name_(name)
		, function_(function)
	{
	}

	Benchmark_F_AG(
		const char *name,
		FunctionT function,
		std::vector<typename GeneratorT::ArgType> args)
		: name_(name)
		, function_(function)
		, args_(std::move(args))
	{
	}

public:
	const std::string& GetName() const
	{
		return name_;
	}

	std::chrono::nanoseconds Measure(size_t)
	{
		const auto& arg = args_[current_arg_index_];

		auto& fix = fixture_opt_.get().SetUp(arg);

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fix, arg);
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

		// args_ is empty means they must be generated with generator,
		// otherwise they are given from origin and must not be changed
		if (args_.empty())
		{
			args_generator_opt_.emplace();

			const auto argc = Env::Instance().GetArgc();
			const auto argv = Env::Instance().GetArgv();
			args_ = args_generator_opt_.get().Generate(argc, argv);
		}

		current_arg_index_ = 0;
	}

	void Finalize()
	{
		fixture_opt_.reset();

		// if args_generator_ exists, we must regenerate args_
		// on the next Prepare, otherwise, leave them as-is
		if (args_generator_opt_.is_initialized())
		{
			args_.clear();
			args_.shrink_to_fit();
			args_generator_opt_.reset();
		}
	}

	bool HasArgsToProcess()
	{
		return current_arg_index_ < args_.size();
	}

	void OnArgProcessed()
	{
		++current_arg_index_;
	}

	std::string CurrentArgAsString()
	{
		std::ostringstream os;
		os << args_[current_arg_index_];
		return os.str();
	}

private:
	std::string name_;
	FunctionT function_;
	scoped_optional<FixtureT> fixture_opt_;
	scoped_optional<GeneratorT> args_generator_opt_;
	std::vector<ArgT> args_;
	size_t current_arg_index_ = 0;
};

} // namespace sltbench
