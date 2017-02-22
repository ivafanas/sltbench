#pragma once

#include "Env.h"
#include "StopGenerationException.h"

#include <chrono>
#include <memory>
#include <sstream>
#include <string>


namespace sltbench {

template<typename FixtureT, typename GeneratorT>
class BenchmarkWithFixtureBuilderAndLazyArgGenerator
{
public:
	typedef typename GeneratorT::ArgType ArgT;
	typedef FixtureT(*FixtureBuilderT)(const ArgT&);
	typedef void(*FunctionT)(FixtureT&, const ArgT&);

public:
	BenchmarkWithFixtureBuilderAndLazyArgGenerator(
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
		const auto& arg = *arg_;

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
		generator_.reset(new GeneratorT(argc, argv));

		PopArg();
	}

	void Finalize()
	{
		generator_ = nullptr;
		arg_ = nullptr;
	}

	bool HasArgsToProcess()
	{
		return arg_ != nullptr;
	}

	void OnArgProcessed()
	{
		PopArg();
	}

	std::string CurrentArgAsString()
	{
		std::ostringstream os;
		os << *arg_;
		return os.str();
	}

private:
	void PopArg()
	{
		try
		{
			arg_.reset(new ArgT(generator_->Generate()));
		}
		catch (const StopGenerationException&)
		{
			arg_ = nullptr;
		}
	}

private:
	std::string name_;
	FunctionT function_;
	FixtureBuilderT builder_;
	std::unique_ptr<GeneratorT> generator_;
	std::unique_ptr<ArgT> arg_;
};

} // namespace sltbench
