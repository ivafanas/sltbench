#pragma once

#include <sltbench/impl/BenchApi.h>

#include <chrono>
#include <functional>
#include <string>


namespace sltbench {

class ProcedureBenchmark
{
public:
	ProcedureBenchmark(const char *name, SLTFun function)
		: name_(name)
		, function_(std::move(function))
	{}

public:
	const std::string& GetName() const
	{
		return name_;
	}

	std::chrono::nanoseconds Measure(size_t)
	{
		const auto start_timepoint = std::chrono::high_resolution_clock::now();
		function_();
		const auto final_timepoint = std::chrono::high_resolution_clock::now();
		return
			final_timepoint > start_timepoint
			? std::chrono::duration_cast<std::chrono::nanoseconds>(final_timepoint - start_timepoint)
			: std::chrono::nanoseconds(0);;
	}

	void Prepare()
	{
	}

	void Finalize()
	{
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
	SLTFun function_;
};

} // namespace sltbenh

