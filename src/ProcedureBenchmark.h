#pragma once

#include <sltbench/impl/BenchApi.h>

#include <chrono>
#include <functional>
#include <string>


namespace sltbench {

class ProcedureBenchmark
{
public:
	ProcedureBenchmark(const char *name, SLTFun function);

public:
	const std::string& GetName() const;
	std::chrono::nanoseconds Measure(size_t);
	void Prepare();
	void Finalize();
	size_t GetArgsCount();
	std::string ConvertArgToString(size_t);

private:
	std::string name_;
	SLTFun function_;
};

} // namespace sltbenh
