#include <sltbench/Bench.h>

#include <iomanip>
#include <iostream>


namespace {

class MyReporter : public sltbench::reporter::IReporter
{
public:
	MyReporter() = default;
	~MyReporter() override = default;

public:
	void ReportBenchmarkStarted() override
	{
		// for example, ignore this event
	}

	void ReportBenchmarkFinished() override
	{
		// for example, ignore this event
	}

	void Report(
		const std::string& name,
		const std::string& params,
		sltbench::Verdict verdict,
		std::chrono::nanoseconds timing_result) override
	{
		std::cout
			<< std::left << std::setw(60) << name
			<< std::left << std::setw(25) << params
			<< std::left << std::setw(9) << ToString(verdict)
			<< std::right << std::setw(20) << timing_result.count() << std::endl;
	}

	void ReportWarning(sltbench::RunWarning warning) override
	{
		// for example, do not report warnings.
	}
};

} // namespace

SLTBENCH_CONFIG().SetReporter(std::unique_ptr<MyReporter>(new MyReporter()));
