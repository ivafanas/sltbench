#pragma once

#include "RunWarnings.h"
#include "Verdict.h"

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
		\param verdict       - runner decision
		\param timing_result - timing result
	 */
	virtual void Report(
		const std::string& name,
		const std::string& params,
		Verdict verdict,
		std::chrono::nanoseconds timing_result) = 0;

	/*!
		Report warning event.

		\param warning - warning kind
	*/
	virtual void ReportWarning(RunWarning warning) = 0;
};

} // namespace reporter
} // namespace sltbench
