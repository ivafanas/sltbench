#include "gtest/googletest/include/gtest/gtest.h"

#include <sltbench/Bench.h>


int main(int argc, char **argv)
{
	sltbench::Env::Instance().SetArgs(argc, argv);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


#include "gtest/googletest/src/gtest-all.cc"
