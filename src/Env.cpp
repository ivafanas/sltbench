#include <sltbench/impl/Env.h>
#include <sltbench/impl/IConfig.h>

#include "Filters.h"

#include <cassert>
#include <cstring>
#include <map>
#include <string>


namespace sltbench {
namespace {


void InitConfig(const int argc, char **argv)
{
	// boost::program_options would be the best solution,
	// but dependency on boost is not so good

	// well, this algorithm will be neither fast neither correct,
	// but correct enough for our case

	std::map<std::string, std::string> options_to_values;
	for (int i = 1; i < argc; ++i)
	{
		char *arg = argv[i];
		size_t arglen = strlen(arg);

		// we are looking for "--option=value" case
		// or "--option value" case

		char *it = std::find(arg, arg + arglen, '=');
		if (it == arg + arglen)
		{
			// case "--option value"
			if (i + 1 < argc)
			{
				options_to_values[std::string(arg, arg + arglen)] = argv[i + 1];
				++i;
			}
		}
		else
		{
			// case "--option=value"
			options_to_values[std::string(arg, it)] = std::string(it + 1, arg + arglen);
		}
	}

	const auto filter_expr = options_to_values["--filter"];
	if (!filter_expr.empty())
	{
		std::regex re(filter_expr, std::regex_constants::basic | std::regex_constants::icase );
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
