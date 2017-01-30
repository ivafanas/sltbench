#pragma once

#include <sltbench/impl/BenchApi.h>

#include <chrono>
#include <string>


namespace sltbench {

class ProcedureBenchmark
{
public:
	ProcedureBenchmark(const char *name, SLTFun function);

public:
	const std::string& GetName() const;
	std::chrono::nanoseconds Measure(size_t calls_count);
	bool SupportsMulticall() const;
	void Prepare();
	void Finalize();
	bool HasArgsToProcess();
	void OnArgProcessed();
	std::string CurrentArgAsString();

private:
	std::string name_;
	SLTFun function_;
	bool measured_ = false;
};

} // namespace sltbenh
