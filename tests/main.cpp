#include "gtest/googletest/include/gtest/gtest.h"


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


#include "gtest/googletest/src/gtest-all.cc"
