#pragma once

#include <map>
#include <string>
#include <vector>


namespace sltbench {

struct ProgramOptions
{
	std::vector<std::string> switches;
	std::map<std::string, std::string> named_values;
};

//! Parse command line args
ProgramOptions ParseCommandLine(int argc, char **argv);

} // namespace sltbench
