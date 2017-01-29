#pragma once

#include <sltbench/impl/IReporter.h>


namespace sltbench {
namespace reporter {

class CsvReporter
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
};

} // namespace reporter
} // namespace sltbench
