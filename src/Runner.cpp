#include "Runner.h"

#include <sltbench/impl/IRunner.h>

#include <cassert>
#include <chrono>
#include <map>
#include <string>


namespace sltbench {
namespace {

std::map<std::string, IRunnerPtr>& Runners()
{
	static std::map<std::string, IRunnerPtr> runners;
	return runners;
}

} // namespace
} // namespace sltbench


namespace sltbench {

void RegisterRunner(const char *tag, const IRunnerPtr& runner)
{
	auto& runners = Runners();
	const std::string name(tag);
	auto it = runners.find(name);
	if (it == runners.end())
	{
		runners[name] = runner;
	}
	else
	{
		assert(it->second == runner);
	}
}

std::vector<IRunnerPtr> GetRunners()
{
	const auto& runners_map = Runners();

	std::vector<IRunnerPtr> rv;
	rv.reserve(runners_map.size());
	for (const auto& name_to_runner : runners_map)
	{
		rv.push_back(name_to_runner.second);
	}
	return rv;
}

} // namespace sltbench
