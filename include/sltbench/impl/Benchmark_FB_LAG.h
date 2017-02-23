#pragma once

#include "Env.h"
#include "Optional.h"
#include "StopGenerationException.h"

#include <chrono>
#include <sstream>
#include <string>


namespace sltbench {

template<typename FixtureT, typename GeneratorT>
class Benchmark_FB_LAG
{
public:
	typedef typename GeneratorT::ArgType ArgT;
	typedef FixtureT(*FixtureBuilderT)(const ArgT&);
	typedef void(*FunctionT)(FixtureT&, const ArgT&);

public:
	Benchmark_FB_LAG(
		const char *name,
		FunctionT function,
		FixtureBuilderT builder)
		: name_(name)
		, function_(function)
		, builder_(builder)
	{
	}

public:
	std::chrono::nanoseconds Measure(size_t)
	{
		const auto& arg = arg_opt_.get();

		auto fixture = builder_(arg);

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fixture, arg);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		const auto rv =
			final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);

		return rv;
	}

	const std::string& GetName() const
	{
		return name_;
	}

	bool SupportsMulticall() const
	{
		return false;
	}

	void Prepare()
	{
		const auto argc = Env::Instance().GetArgc();
		const auto argv = Env::Instance().GetArgv();
		generator_opt_.emplace(argc, argv);

		PopArg();
	}

	void Finalize()
	{
		generator_opt_.reset();
		arg_opt_.reset();
	}

	bool HasArgsToProcess()
	{
		return arg_opt_.is_initialized();
	}

	void OnArgProcessed()
	{
		PopArg();
	}

	std::string CurrentArgAsString()
	{
		std::ostringstream os;
		os << arg_opt_.get();
		return os.str();
	}

private:
	void PopArg()
	{
		try
		{
			arg_opt_.emplace(generator_opt_.get().Generate());
		}
		catch (const StopGenerationException&)
		{
			arg_opt_.reset();
		}
	}

private:
	std::string name_;
	FunctionT function_;
	FixtureBuilderT builder_;
	scoped_optional<GeneratorT> generator_opt_;
	scoped_optional<ArgT> arg_opt_;
};

} // namespace sltbench
