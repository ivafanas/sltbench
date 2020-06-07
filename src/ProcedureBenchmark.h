#pragma once

#include <sltbench/impl/BenchApi.h>
#include <sltbench/impl/IBenchmark.h>

#include <chrono>
#include <string>


namespace sltbench {

class ProcedureBenchmark : public IBenchmark
{
public:
	ProcedureBenchmark(const char *name, SLTFun function);

public:
	std::chrono::nanoseconds Measure(size_t calls_count) override;
	void Prepare() override;
	void Finalize() override;
	bool HasArgsToProcess() override;
	void OnArgProcessed() override;
	std::string CurrentArgAsString() override;

private:
	SLTFun function_;
	bool measured_ = false;
};

} // namespace sltbenh
