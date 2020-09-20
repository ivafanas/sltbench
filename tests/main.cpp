#include "gtest/googletest/include/gtest/gtest.h"

#include <sltbench/Bench.h>

#include "src/EnvImpl.h"


int main(int argc, char **argv)
{
	sltbench::SetArgs(argc, argv);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


#include "gtest/googletest/src/gtest-all.cc"
