#include "JsonReporter.h"

#include <iostream>


namespace sltbench {
namespace reporter {

JsonReporter::JsonReporter()
{
	std::cout << "[" << std::endl;
}

JsonReporter::~JsonReporter()
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

	std::cout << "{\n";
	std::cout << "  \"name\": \"" << name << "\",\n";
	std::cout << "  \"arg\": \"" << params << "\",\n";
	std::cout << "  \"status\": \"" << (ok ? "ok" : "CRASHED") << "\",\n";
	std::cout << "  \"time(ns)\": " << timing_result.count() << '\n';
	std::cout << "}" << std::endl;

	is_any_result_reported_ = true;
}

} // namespace repoerter
} // namespace sltbench