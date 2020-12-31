#include <sltbench/impl/BenchApi.h>
#include <sltbench/impl/Verdict.h>

#include "BenchmarksContainerPrivate.h"
#include "Config.h"
#include "EnvImpl.h"
#include "ProcedureBenchmark.h"
#include "Reporters.h"

#include <chrono>
#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>


using namespace sltbench;

static void heatup_function()
{
	// some unuseful staff here
	for (int i = 3; i != 10; ++i)
		for (int j = 3; j != 10; ++j)
			for (int k = 3; k != 10; ++k)
				if (i * i * i + j * j * j == k * k * k)
					std::puts("Fermat theorem has been disproven!");
}

static void heatup()
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

static void ProcessRunWarnings()
{
#ifndef NDEBUG
	auto& config = Config::Instance();

	const bool should_benchmark = !config.list_benchmarks;
	if (should_benchmark)
		config.GetReporter().ReportWarning(RunWarning::DEBUG_BUILD);
#endif
}

namespace {
	struct RunResult
	{
		std::chrono::nanoseconds time_ns;
		Verdict verdict = Verdict::OK;
	};
}  // namespace

static RunResult Run(IBenchmark& bm)
{
	RunResult rv;

	// user-defined functions might throw exceptions
	// we should fail gracefully
	try
	{
		const auto measure_func = [&bm](size_t calls_count) {
			return bm.Measure(calls_count);
		};

		// estimate execution time
		const auto estimation = Estimate(measure_func, bm.supports_multicall);

		if (estimation.verdict == EstimationResult::Verdict::CANNOT_BE_PRECISE)
			rv.verdict = Verdict::IMPRECISE;

		rv.time_ns = Measure(measure_func, estimation);
	}
	catch (const std::exception&)
	{
		rv.time_ns = std::chrono::nanoseconds(0);
		rv.verdict = Verdict::CRASHED;
	}
	return rv;
}

static bool Run(IBenchmark& bm, reporter::IReporter& reporter)
{
	bm.Prepare();

	bool ok = true;
	while (bm.HasArgsToProcess())
	{
		const auto res = Run(bm);

		reporter.Report(
			bm.name,
			bm.CurrentArgAsString(),
			res.verdict,
			res.time_ns);

		if (res.verdict == Verdict::CRASHED)
			ok = false;

		bm.OnArgProcessed();
	}

	bm.Finalize();

	return ok;
}

static void ListBenchmark(IBenchmark& bm)
{
	bm.Prepare();
	while (bm.HasArgsToProcess())
	{
		const std::string arg_str = bm.CurrentArgAsString();
		if (!arg_str.empty())
			std::printf("%s/%s\n", bm.name, arg_str.c_str());
		else
			std::puts(bm.name);
		bm.OnArgProcessed();
	}
	bm.Finalize();
}

static void ListBenchmarks()
{
	for (auto* benchmark : GetRegisteredBenchmarks())
		ListBenchmark(*benchmark);
}

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
	SetArgs(argc, argv);
}

//! @return 0 is all runners successfully completed, 1 otherwise
int Run()
{
	ProcessRunWarnings();

	auto& config = Config::Instance();

	// enumerate registered benchmarks and quit in |list| mode
	if (config.list_benchmarks)
	{
		ListBenchmarks();
		return 0;
	}

	if (config.is_heatup_required)
	{
		// some kind of dark magic: heatup core and scheduler
		heatup();
	}

	bool was_crash = false;
	auto& reporter = config.GetReporter();
	auto* filter = config.filter.get_ptr();

	reporter.ReportBenchmarkStarted();
	for (auto* benchmark : GetRegisteredBenchmarks())
	{
		if (filter && !filter->ShouldRunBenchmark(benchmark->name))
			continue;

		const bool ok = ::Run(*benchmark, reporter);
		was_crash = was_crash || !ok;
	}
	reporter.ReportBenchmarkFinished();
	return was_crash ? 1 : 0;
}

Descriptor* RegisterBenchmark(const char *name, SLTFun func)
{
	RegisterBenchmark(new ProcedureBenchmark(name, func));

	static Descriptor dscr;
	return &dscr;
}

IConfig& GetConfig()
{
	return Config::Instance();
}

} // namespace sltbench

