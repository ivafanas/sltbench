#include <sltbench/impl/Env.h>

#include "EnvImpl.h"

#include "Config.h"
#include "Filters.h"
#include "ProgramOptions.h"
#include "Reporters.h"

#include <iostream>
#include <memory>
#include <stdexcept>


static int g_argc = -1;
static char** g_argv = nullptr;


static void ensureCmdLineArgsInitialized()
{
	if (g_argc == -1)
		throw std::runtime_error("command line args are not initialized yet");
}

namespace sltbench {
namespace {

template<typename T>
std::unique_ptr<T> make_uniq()
{
	return std::unique_ptr<T>(new T);
}

void InitConfig(const int argc, char **argv)
{
	auto options_to_values = BuildProgramOptions(argc, argv);

	// setup test cases filter
	const auto filter_expr = options_to_values["--filter"];
	if (!filter_expr.empty())
	{
		std::regex re(
			filter_expr,
			std::regex_constants::basic | std::regex_constants::icase );
		Config::Instance().filter.reset(new RegexFilter(std::move(re)));
	}

	// setup if heatup required
	const auto heatup_value = options_to_values["--heatup"];
	if (!heatup_value.empty())
	{
		if (heatup_value == "off" || heatup_value == "OFF")
			Config::Instance().is_heatup_required = false;
		else if (heatup_value == "on" || heatup_value == "ON")
			Config::Instance().is_heatup_required = true;
		else
		{
			std::cerr << "ERROR: Unknown heatup option value: " << heatup_value << '\n';
			throw std::runtime_error("invalid command line options");
		}
	}

	// setup output format
	const auto reporter_value = options_to_values["--reporter"];
	if (!reporter_value.empty())
	{
		if (reporter_value == "csv")
			Config::Instance().SetReporter(make_uniq<reporter::CsvReporter>());
		else if (reporter_value == "json")
			Config::Instance().SetReporter(make_uniq<reporter::JsonReporter>());
		else if (reporter_value == "console")
			Config::Instance().SetReporter(make_uniq<reporter::ConsoleReporter>());
		else
		{
			std::cerr << "ERROR: unknown reporter: " << reporter_value << '\n';
			throw std::runtime_error("invalid command line options");
		}
	}
}

} // namespace
} // namespace sltbench


namespace sltbench {

void SetArgs(int argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	InitConfig(argc, argv);
}

int GetArgc()
{
	ensureCmdLineArgsInitialized();

	return g_argc;
}

char** GetArgv()
{
	ensureCmdLineArgsInitialized();

	return g_argv;
}

} // namespace sltbench
