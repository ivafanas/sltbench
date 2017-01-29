#pragma once

#include "Env.h"
#include "StopGenerationException.h"

#include <chrono>
#include <memory>
#include <sstream>
#include <string>


namespace sltbench {

template<typename GeneratorT>
class BenchmarkWithLazyArgGenerator
{
public:
    typedef typename GeneratorT::ArgType ArgT;
    typedef void (*FunctionT)(const ArgT &);

public:
    BenchmarkWithLazyArgGenerator(const char *name, FunctionT function)
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
		const auto& arg = *arg_;

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
	std::unique_ptr<GeneratorT> generator_;
    std::unique_ptr<ArgT> arg_;
};

} // namespace sltbench
