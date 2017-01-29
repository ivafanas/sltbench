#pragma once

#include "Env.h"
#include "StopGenerationException.h"

#include <chrono>
#include <sstream>
#include <string>


namespace sltbench {

template<typename FixtureT, typename GeneratorT>
class BenchmarkWithFixtureAndLazyArgGenerator
{
public:
    typedef typename FixtureT::Type FixT;
    typedef typename GeneratorT::ArgType ArgT;
    typedef void (*FunctionT)(FixT&, const ArgT&);

public:
	BenchmarkWithFixtureAndLazyArgGenerator(const char *name, FunctionT function)
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
		const auto& arg = *arg_;

		auto& fix = fixture_->SetUp(arg);

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(fix, arg);
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

        const auto argc = Env::Instance().GetArgc();
        const auto argv = Env::Instance().GetArgv();
		generator_.reset(new GeneratorT(argc, argv));

		PopArg();
	}

	void Finalize()
	{
		fixture_ = nullptr;
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
	std::unique_ptr<FixtureT> fixture_;
	FunctionT function_;
	std::unique_ptr<GeneratorT> generator_;
    std::unique_ptr<ArgT> arg_;
};

} // namespace sltbench
