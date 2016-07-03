#pragma once

#include <map>
#include <string>


namespace sltbench {

//! Build option-to-value map from command line args
std::map<std::string, std::string> BuildProgramOptions(int argc, char **argv);

} // namespace sltbench
