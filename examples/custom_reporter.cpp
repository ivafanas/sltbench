#include <sltbench/Bench.h>

#include "ConsoleReporter.h"


namespace {

// custom reporter.
// just forward call to ConsoleReporter, but method can do whatever you want
class MyReporter : public sltbench::reporter::IReporter
{
public:
	MyReporter() = default;
	~MyReporter() override = default;

public:
	void ReportBenchmarkStarted() override
	{
		console_reporter_.ReportBenchmarkStarted();
	}

	void ReportBenchmarkFinished() override
	{
		console_reporter_.ReportBenchmarkFinished();
	}

	void Report(
		const std::string& name,
		const std::string& params,
		sltbench::Verdict verdict,
		std::chrono::nanoseconds timing_result) override
	{
		// std::cout << name << params << ok << timing_result.count() << std::endl;
		console_reporter_.Report(name, params, verdict, timing_result);
	}

private:
	sltbench::reporter::ConsoleReporter console_reporter_;
};

} // namespace

SLTBENCH_CONFIG().SetReporter(std::unique_ptr<MyReporter>(new MyReporter()));
