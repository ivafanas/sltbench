#pragma once

#include <chrono>
#include <string>


namespace sltbench {
namespace reporter {

struct IReporter
{
	virtual ~IReporter() {}

	/*!
		Report timing started.
		Called once per exe-run.
	 */
	virtual void ReportBenchmarkStarted() = 0;

	/*!
		Report timing finished.
		Called once per exe-run.
	 */
	virtual void ReportBenchmarkFinished() = 0;

	/*!
		Report timing result.
		Called between ReportBenchmarkStarted and ReportBenchmarkStarted.

		\param name          - name
		\param params        - input string representation
		\param ok            - whether timing succeeded or failed
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
