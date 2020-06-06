#include "Reporters.h"

#include <cstdio>
#include <iomanip>
#include <iostream>


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
	std::cout
		<< std::left << std::setw(60) << "benchmark"
		<< std::left << std::setw(25) << "arg"
		<< std::left << std::setw(9) << "status"
		<< std::right << std::setw(20) << "time(ns)" << std::endl;
}

void ConsoleReporter::ReportBenchmarkFinished()
{
}

void ConsoleReporter::Report(
	const std::string& name,
	const std::string& params,
	Verdict verdict,
	std::chrono::nanoseconds timing_result)
{
	std::cout
		<< std::left << std::setw(60) << name
		<< std::left << std::setw(25) << params
		<< std::left << std::setw(9) << ToString(verdict)
		<< std::right << std::setw(20) << timing_result.count() << std::endl;
}

void ConsoleReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

void CsvReporter::ReportBenchmarkStarted()
{
	std::cout << "benchmark,arg,status,time(ns)\n";
}

void CsvReporter::ReportBenchmarkFinished()
{
}

void CsvReporter::Report(
	const std::string& name,
	const std::string& params,
	Verdict verdict,
	std::chrono::nanoseconds timing_result)
{
	std::cout
		<< name << ','
		<< params << ','
		<< ToString(verdict) << ','
		<< timing_result.count() << std::endl;
}

void CsvReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

void JsonReporter::ReportBenchmarkStarted()
{
	std::cout << "[\n";
}

void JsonReporter::ReportBenchmarkFinished()
{
	std::cout << "]" << std::endl;
}

void JsonReporter::Report(
	const std::string& name,
	const std::string& params,
	Verdict verdict,
	std::chrono::nanoseconds timing_result)
{
	if (is_any_result_reported_)
	{
		std::cout << ",\n";
	}

	std::cout
		<< "{\n"
		<< "  \"name\": \"" << name << "\",\n"
		<< "  \"arg\": \"" << params << "\",\n"
		<< "  \"status\": \"" << ToString(verdict) << "\",\n"
		<< "  \"time(ns)\": " << timing_result.count() << '\n'
		<< "}" << std::endl;

	is_any_result_reported_ = true;
}

void JsonReporter::ReportWarning(const RunWarning warning)
{
	PrintWarning(warning);
}

} // namespace reporter
} // namespace sltbench
