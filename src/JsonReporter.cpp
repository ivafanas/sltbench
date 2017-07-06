#include "JsonReporter.h"

#include "WarningsMessages.h"

#include <iostream>


namespace sltbench {
namespace reporter {

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

void JsonReporter::ReportWarning(RunWarning warning)
{
	std::cerr << "WARNING: " << MessageForWarning(warning) << std::endl;
}

} // namespace repoerter
} // namespace sltbench
