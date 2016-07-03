#include <sltbench/impl/Env.h>
#include <sltbench/impl/IConfig.h>

#include "Filters.h"
#include "ProgramOptions.h"

#include <cassert>
#include <memory>
#include <map>
#include <string>


namespace sltbench {
namespace {

void InitConfig(const int argc, char **argv)
{
	auto options_to_values = BuildProgramOptions(argc, argv);

	const auto filter_expr = options_to_values["--filter"];
	if (!filter_expr.empty())
	{
		std::regex re(
			filter_expr,
			std::regex_constants::basic | std::regex_constants::icase );
		auto filter = std::unique_ptr<IFilter>(new RegexFilter(std::move(re)));
		GetConfig().GetPrivate().SetFilter(std::move(filter));
	}
}

} // namespace
} // namespace sltbench


namespace sltbench {

Env& Env::Instance()
{
	static Env inst;
	return inst;
}

void Env::SetArgs(const int argc, char **argv)
{
	argc_ = argc;
	argv_ = argv;

	InitConfig(argc, argv);
}

int Env::GetArgc() const
{
	assert(argc_ != 0);
	return argc_;
}

char **Env::GetArgv() const
{
	assert(argv_ != 0);
	return argv_;
}

} // namespace sltbench
