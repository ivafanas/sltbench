#pragma once

#include <chrono>
#include <string>


namespace sltbench {
namespace reporter {

struct IReporter
{
	virtual ~IReporter() {}

	/*!
		Report timing result
	 
		\param name - benchmark name
		\param params - input string representation
		\param ok - whether timing succeeded or failed
		\param timing_result - timing result
	 */
	virtual void Report(
		const std::string& name,
		const std::string& params,
		bool ok,
		std::chrono::nanoseconds timing_result) = 0;
};

} // namespace reporter
} // namespace sltbench