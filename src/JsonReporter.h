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
	virtual void Report(
		const std::string& name,
		const std::string& params,
		bool ok,
		std::chrono::nanoseconds timing_result) override;

private:
	bool is_any_result_reported_ = false;
};

} // namespace reporter
} // namespace sltbench
