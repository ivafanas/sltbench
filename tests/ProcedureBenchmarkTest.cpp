#include "gtest/gtest.h"

#include "src/ProcedureBenchmark.h"


TEST(ProcedureBenchmark, GetNameReturnsBenchmarkName)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_EQ("name", bm.GetName());
}

TEST(ProcedureBenchmark, HasArgsToProcessReturnsTrue)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(ProcedureBenchmark, HasArgsToProcessReturnsFalseAfterProcess)
{
    sltbench::ProcedureBenchmark bm("name", []() {});

    bm.OnArgProcessed();

    EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(ProcedureBenchmark, ConvertArgToStringReturnsEmptyString)
{
    sltbench::ProcedureBenchmark bm("name", [](){});

    EXPECT_TRUE(bm.CurrentArgAsString().empty());
}

TEST(ProcedureBenchmark, MeasureShouldCallFunctionOnce)
{
    size_t n_calls = 0;
    sltbench::ProcedureBenchmark bm("name", [&](){ ++n_calls; });

    bm.Measure();

    EXPECT_EQ(1, n_calls);
}
