#include <sltbench/Bench.h>

#include "ConsoleReporter.h"


namespace {

// custom reporter.
// just forward call to ConsoleReporter, but method can do whatever you want
class MyReporter : public sltbench::reporter::IReporter
{
public:
	MyReporter() = default;
	virtual ~MyReporter() override = default;

public:
	virtual void Report(
		const std::string& name,
		const std::string& params,
		bool ok,
		std::chrono::nanoseconds timing_result) override
	{
		// std::cout << name << params << ok << timing_result.count() << std::endl;
		consoleReporter_.Report(name, params, ok, timing_result);
	}

private:
	sltbench::reporter::ConsoleReporter consoleReporter_;
};

} // namespace

SLTBENCH_CONFIG().SetReporter(std::unique_ptr<MyReporter>(new MyReporter()));


