#pragma once

#include <sltbench/impl/IReporter.h>


namespace sltbench {
namespace reporter {

class ConsoleReporter
	: public IReporter
{
public: // IReporter
	void ReportBenchmarkStarted() override;
	void ReportBenchmarkFinished() override;
	void Report(
		const std::string& name,
		const std::string& params,
		Verdict verdict,
		std::chrono::nanoseconds timing_result) override;
	void ReportWarning(RunWarning warning) override;
};

} // namespace reporter
} // namespace sltbench
