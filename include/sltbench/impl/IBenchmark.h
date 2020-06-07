#pragma once

#include <chrono>
#include <string>


namespace sltbench {

class IBenchmark
{
public:
	IBenchmark(const char* name, bool supports_multicall) noexcept;
	IBenchmark(const IBenchmark&) = delete;
	IBenchmark(IBenchmark&&) noexcept = delete;
	IBenchmark& operator=(const IBenchmark&) = delete;
	IBenchmark& operator=(IBenchmark&) noexcept = delete;
	virtual ~IBenchmark() noexcept;

	virtual std::chrono::nanoseconds Measure(size_t calls_count) = 0;
	virtual void Prepare() = 0;
	virtual void Finalize() = 0;
	virtual bool HasArgsToProcess() = 0;
	virtual void OnArgProcessed() = 0;
	virtual std::string CurrentArgAsString() = 0;

	const char* const name;
	const bool supports_multicall;
};

}  // namespace sltbench

