#include <sltbench/impl/BenchApi.h>
#include <sltbench/impl/Env.h>
#include <sltbench/impl/IRunner.h>

#include "Config.h"
#include "ConsoleReporter.h"
#include "ProcedureBenchmark.h"
#include "Runner.h"

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
	const std::chrono::nanoseconds heatup_limit = std::chrono::seconds(7);
	std::chrono::nanoseconds heating_time(0);
	while (heating_time < heatup_limit)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		heatup_function();
		auto final_time = std::chrono::high_resolution_clock::now();
		heating_time += (final_time - start_time);
	}
}

} // namespace
} // namespace sltbench



namespace sltbench {


void Init(int argc, char **argv)
{
	Env::Instance().SetArgs(argc, argv);
}


int Run(const bool heatup_required)
{
	if (heatup_required)
	{
		// some kind of dark magic: heatup core and scheduler
		heatup();
	}

	bool was_crash = false;
	auto& reporter = Config::Instance().GetReporter();
	auto& filter = Config::Instance().GetFilter();
	const auto runners = GetRunners();
	for (const auto& runner : runners)
	{
		const bool crashed = runner->Run(reporter, filter);
		if (crashed)
			was_crash = true;
	}
	return was_crash ? 1 : 0;
}


Descriptor* RegisterBenchmark(const char *name, SLTFun func)
{
	using BM = ProcedureBenchmark;

	Runner<BM>::Register();
	BenchmarksContainer<BM>::Instance().Add(BM(name, func));

	static Descriptor dscr;
	return &dscr;
}


IConfig& GetConfig()
{
	return Config::Instance();
}

} // namespace sltbench