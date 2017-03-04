#pragma once

#include <sltbench/impl/IReporter.h>


namespace sltbench {
namespace reporter {

class JsonReporter
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

private:
	bool is_any_result_reported_ = false;
};

} // namespace reporter
} // namespace sltbench
