#include "gtest/gtest.h"

#include "src/ProcedureBenchmark.h"


TEST(ProcedureBenchmark, GetNameReturnsBenchmarkName)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_EQ("name", bm.GetName());
}

TEST(ProcedureBenchmark, GetArgsCountReturnsOne)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_EQ(1, bm.GetArgsCount());
}

TEST(ProcedureBenchmark, ConvertArgToStringReturnsEmptyString)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_TRUE(bm.ConvertArgToString(0).empty());
}

TEST(ProcedureBenchmark, MeasureShouldCallFunctionOnce)
{
    size_t n_calls = 0;
    sltbench::ProcedureBenchmark bm("name", [&](){ ++n_calls; });

    bm.Measure(0);

    EXPECT_EQ(1, n_calls);
}
