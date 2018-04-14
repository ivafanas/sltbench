#include <sltbench/impl/BenchApi.h>
#include <sltbench/impl/Env.h>
#include <sltbench/impl/IRunner.h>

#include "Config.h"
#include "ConsoleReporter.h"
#include "ProcedureBenchmark.h"
#include "Runner.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


namespace sltbench {
namespace {

void heatup_function()
{
	// some unuseful staff here
	const size_t count = 1024 * 1024;
	std::vector<unsigned> vec;
	vec.reserve(count);
	for (size_t i = 0; i < count; ++i)
		vec.push_back(i % 1024);
	vec.erase(vec.begin());
}

void heatup()
{
	const std::chrono::nanoseconds heatup_limit = std::chrono::seconds(3);
	std::chrono::nanoseconds heating_time(0);
	while (heating_time < heatup_limit)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		heatup_function();
		auto final_time = std::chrono::high_resolution_clock::now();
		heating_time += (final_time - start_time);
	}
}

bool IsCompiledInDebug()
{
#ifndef NDEBUG
	return true;
#else
	return false;
#endif
}

void ProcessRunWarnings()
{
	auto& reporter = Config::Instance().GetReporter();

	if (IsCompiledInDebug())
		reporter.ReportWarning(RunWarning::DEBUG_BUILD);
}

} // namespace
} // namespace sltbench



namespace sltbench {

int Main(int argc, char **argv)
{
	try
	{
		Init(argc, argv);
		return Run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "ERROR: unknown exception" << std::endl;
	}
	return 1;
}

void Init(int argc, char **argv)
{
	Env::Instance().SetArgs(argc, argv);
}

//! @return 0 is all runners successfully completed, 1 otherwise
int Run()
{
	ProcessRunWarnings();

	if (GetConfig().GetPrivate().IsHeatupRequired())
	{
		// some kind of dark magic: heatup core and scheduler
		heatup();
	}

	bool was_crash = false;
	auto& reporter = Config::Instance().GetReporter();
	auto& filter = Config::Instance().GetFilter();

	reporter.ReportBenchmarkStarted();
	const auto runners = GetRunners();
	for (const auto& runner : runners)
	{
		const bool ok = runner->Run(reporter, filter);
		was_crash |= !ok;
	}
	reporter.ReportBenchmarkFinished();
	return was_crash ? 1 : 0;
}

Descriptor* RegisterBenchmark(const char *name, SLTFun func)
{
	using BM = ProcedureBenchmark;

	Runner<BM>::Register();
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

	static Descriptor dscr;
	return &dscr;
}

IConfig& GetConfig()
{
	return Config::Instance();
}

} // namespace sltbench
