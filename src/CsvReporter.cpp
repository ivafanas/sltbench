#include "CsvReporter.h"

#include <iostream>


namespace sltbench {
namespace reporter {

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
	bool ok,
	std::chrono::nanoseconds timing_result)
{
	std::cout
		<< name << ','
		<< params << ','
		<< (ok ? "ok" : "CRASHED") << ','
		<< timing_result.count() << std::endl;
}

} // namespace repoerter
} // namespace sltbench
