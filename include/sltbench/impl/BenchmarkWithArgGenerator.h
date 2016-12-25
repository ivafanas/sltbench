#pragma once

#include "Env.h"

#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace sltbench {

template<typename GeneratorT>
class BenchmarkWithArgGenerator
{
public:
	typedef std::function<void(const typename GeneratorT::ArgType &)> FunctionT;

public:
	BenchmarkWithArgGenerator(const char *name, FunctionT function)
		: name_(name)
		, function_(std::move(function))
	{
	}

	BenchmarkWithArgGenerator(
        const char *name,
        FunctionT function,
        std::vector<typename GeneratorT::ArgType> args)
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

	std::chrono::nanoseconds Measure()
	{
		const auto& arg = args_[current_arg_index_];

		const auto start_ts = std::chrono::high_resolution_clock::now();
		function_(arg);
		const auto final_ts = std::chrono::high_resolution_clock::now();
		return final_ts > start_ts
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_ts - start_ts)
			: std::chrono::nanoseconds(0);
	}

	void Prepare()
	{
        // args_ is empty means they must be generated with generator,
        // otherwise they are given from origin and must not be changed
		if (args_.empty())
		{
            args_generator_.reset(new GeneratorT());

            const auto argc = Env::Instance().GetArgc();
            const auto argv = Env::Instance().GetArgv();
			args_ = args_generator_->Generate(argc, argv);
		}

        current_arg_index_ = 0;
	}

	void Finalize()
	{
        // if args_generator_ exists, we must regenerate args_
        // on the next Prepare, otherwise, leave them as-is
		if (args_generator_)
		{
			args_.clear();
			args_.shrink_to_fit();
			args_generator_.reset();
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
	std::unique_ptr<GeneratorT> args_generator_;
	std::vector<typename GeneratorT::ArgType> args_;
    size_t current_arg_index_ = 0;
};

} // namespace sltbench
