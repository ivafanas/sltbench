#include "Reporters.h"

#include <cstdio>
#include <type_traits>


namespace sltbench {
namespace reporter {

static void PrintWarning(const RunWarning)
{
	// there is just a single warning in the library now
	std::fputs("WARNING: sltbench is compiled in debug mode. "
	           "Performance results might be not relevant.\n",
	           stderr);
}

void ConsoleReporter::ReportBenchmarkStarted()
{
	std::printf("%-60s%-25s%-9s%20s\n", "benchmark", "arg", "status", "time(ns)");
}

void ConsoleReporter::ReportBenchmarkFinished()
{
}

void ConsoleReporter::Report(
	const std::string& name,
	const std::string& params,
	const Verdict verdict,
	const std::chrono::nanoseconds timing_result)
{
	const std::string verdict_str = ToString(verdict);
	const long long timing_result_value =
		static_cast<long long>(timing_result.count());
	std::printf(
		"%-60s%-25s%-9s%20lld\n",
		name.c_str(),
		params.c_str(),
		verdict_str.c_str(),
		timing_result_value);
}

void ConsoleReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

void CsvReporter::ReportBenchmarkStarted()
{
	std::puts("benchmark,arg,status,time(ns)");
}

void CsvReporter::ReportBenchmarkFinished()
{
}

void CsvReporter::Report(
	const std::string& name,
	const std::string& params,
	const Verdict verdict,
	const std::chrono::nanoseconds timing_result)
{
	const std::string verdict_str = ToString(verdict);
	const long long timing_result_value =
		static_cast<long long>(timing_result.count());
	std::printf(
		"%s,%s,%s,%lld\n",
		name.c_str(),
		params.c_str(),
		verdict_str.c_str(),
		timing_result_value);
}

void CsvReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

void JsonReporter::ReportBenchmarkStarted()
{
	std::puts("[");
}

void JsonReporter::ReportBenchmarkFinished()
{
	std::puts("]");
}

void JsonReporter::Report(
	const std::string& name,
	const std::string& params,
	const Verdict verdict,
	const std::chrono::nanoseconds timing_result)
{
	if (is_any_result_reported_)
		std::puts(",");

	const std::string verdict_str = ToString(verdict);
	const long long timing_result_value =
		static_cast<long long>(timing_result.count());
	std::printf(
		"{\n"
		"  \"name\": \"%s\",\n"
		"  \"arg\": \"%s\",\n"
		"  \"status\": \"%s\",\n"
		"  \"time(ns)\": %lld\n"
		"}\n",
		name.c_str(),
		params.c_str(),
		verdict_str.c_str(),
		timing_result_value);

	is_any_result_reported_ = true;
}

void JsonReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

} // namespace reporter
} // namespace sltbench
