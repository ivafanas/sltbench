#pragma once

#include "Env.h"
#include "Optional.h"
#include "StopGenerationException.h"

#include <chrono>
#include <sstream>
#include <string>


namespace sltbench {

template<typename GeneratorT>
class Benchmark_LAG
{
public:
	typedef typename GeneratorT::ArgType ArgT;
	typedef void (*FunctionT)(const ArgT &);

public:
	Benchmark_LAG(const char *name, FunctionT function)
		: name_(name)
		, function_(function)
	{
	}

public:
	const std::string& GetName() const
	{
		return name_;
	}

	std::chrono::nanoseconds Measure(size_t calls_count)
	{
		const auto& arg = arg_opt_.get();

		const auto start_ts = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < calls_count; ++i)
			function_(arg);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		return final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);
	}

	bool SupportsMulticall() const
	{
		return true;
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
	scoped_optional<GeneratorT> generator_opt_;
	scoped_optional<ArgT> arg_opt_;
};

} // namespace sltbench
