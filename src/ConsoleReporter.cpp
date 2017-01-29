#include "ConsoleReporter.h"

#include <iomanip>
#include <iostream>


namespace sltbench {
namespace reporter {

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

} // namespace repoerter
} // namespace sltbench
