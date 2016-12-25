#include <sltbench/impl/Env.h>
#include <sltbench/impl/IConfig.h>

#include "Filters.h"
#include "ProgramOptions.h"

#include <memory>


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
	return argc_;
}

char **Env::GetArgv() const
{
	return argv_;
}

} // namespace sltbench
