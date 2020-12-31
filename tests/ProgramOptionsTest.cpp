#include "gtest/gtest.h"

#include "src/Algorithms.h"
#include "src/ProgramOptions.h"

#include <iostream>
#include <string>
#include <vector>


using namespace sltbench;

namespace {

struct Param
{
	std::vector<std::string> args;
	std::vector<std::string> expected_switches;
	std::map<std::string, std::string> expected_named_values;
};

using ProgramOptionsParamTests = testing::TestWithParam<Param>;

}  // namespace

static ProgramOptions Parse(const std::vector<std::string>& args)
{
	const int argc = static_cast<int>(args.size());

	std::vector<char*> argv(argc, nullptr);
	for (int i = 0; i < argc; ++i)
		argv[i] = (char *)args[i].c_str();

	return ParseCommandLine(argc, argv.data());
}

TEST_P(ProgramOptionsParamTests, Check)
{
	const auto options = Parse(GetParam().args);
	EXPECT_EQ(options.switches, GetParam().expected_switches);
	EXPECT_EQ(options.named_values, GetParam().expected_named_values);
}

INSTANTIATE_TEST_CASE_P(
	ProgramOptionsTest,
	ProgramOptionsParamTests,
	testing::Values(
		Param{/*args*/{"bench.exe"},
		      /*switches*/{}, /*values*/{}
		},
		Param{/*args*/{ "bench.exe", "--filter=.*ho" },
		      /*switches*/{}, /*values*/{{"--filter", ".*ho"}}
		},
		Param{/*args*/{ "bench.exe", "--filter", ".*ho" },
		      /*switches*/{}, /*values*/{{"--filter", ".*ho"}}
		},
		Param{/*args*/{ "bench.exe", "arg", "--filter", ".*ho" },
		      /*switches*/{}, /*values*/{{"--filter", ".*ho"}}
		},
		Param{/*args*/{ "bench.exe", "-p", "--filter", ".*ho" },
		      /*switches*/{}, /*values*/{{"--filter", ".*ho"}}
		},
		Param{/*args*/{ "bench.exe", "-p", "p", "--filter", ".*ho" },
		      /*switches*/{}, /*values*/{{"--filter", ".*ho"}}
		},
		Param{/*args*/{ "bench.exe", "--switch" },
		      /*switches*/{ "--switch" }, /*values*/{}
		},
		Param{/*args*/{ "bench.exe", "--switch", "--key=value" },
		      /*switches*/{ "--switch" }, /*values*/{{"--key", "value"}}
		},
		Param{/*args*/{ "bench.exe", "--switch", "--key", "value" },
		      /*switches*/{ "--switch" }, /*values*/{{"--key", "value"}}
		},
		Param{/*args*/{ "bench.exe", "--key=value", "--switch" },
		      /*switches*/{ "--switch" }, /*values*/{{"--key", "value"}}
		},
		Param{/*args*/{ "bench.exe", "--key", "value", "--switch" },
		      /*switches*/{ "--switch" }, /*values*/{{"--key", "value"}}
		},
		Param{/*args*/{ "benchmark.exe", "--switch1", "--switch2" },
		      /*switches*/{ "--switch1", "--switch2" }, /*values*/{}
		}));

