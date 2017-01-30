#include "ProgramOptions.h"

#include <algorithm>
#include <cstring>


namespace sltbench {

std::map<std::string, std::string> BuildProgramOptions(int argc, char **argv)
{
	// boost::program_options would be the best solution,
	// but dependency on boost is not so good

	// well, this algorithm will be neither fast neither correct,
	// but correct enough for our case

	std::map<std::string, std::string> rv;
	for (int i = 1; i < argc; ++i)
	{
		char *arg = argv[i];
		size_t arglen = strlen(arg);

		// we are looking for "--option=value" case
		// or "--option value" case
		//
		// we are not interested in positional parameters
		// and parameters like "-o" and "--option"

		char *it = std::find(arg, arg + arglen, '=');
		if (it == arg + arglen)
		{
			if (i + 1 < argc)
			{
				if (arglen > 2 && arg[0] == '-' && arg[1] == '-')
				{
					char *next_arg = argv[i + 1];
					if (next_arg[0] != '-')
					{
						// case "--option value"
						rv[std::string(arg, arg + arglen)] = argv[i + 1];
						++i;
					}
					// else - case "--option" without value -- skip it
				}
				// else - case of positional parameter or "-o" option -- skip it
			}
			// else - case "-o" or "--option" without value -- skip it
		}
		else
		{
			// case "--option=value"
			rv[std::string(arg, it)] = std::string(it + 1, arg + arglen);
		}
	}

	return rv;
}

} // namespace sltbench
