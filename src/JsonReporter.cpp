#include "JsonReporter.h"

#include <iostream>


namespace sltbench {
namespace reporter {

void JsonReporter::ReportBenchmarkStarted()
{
	std::cout << "[\n";
}

void JsonReporter::ReportBenchmarkFinished()
{
	std::cout << "]" << std::flush;
}

void JsonReporter::Report(
	const std::string& name,
	const std::string& params,
	bool ok,
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
		<< "  \"status\": \"" << (ok ? "ok" : "CRASHED") << "\",\n"
		<< "  \"time(ns)\": " << timing_result.count() << '\n'
		<< "}" << std::endl;

	is_any_result_reported_ = true;
}

} // namespace repoerter
} // namespace sltbench
