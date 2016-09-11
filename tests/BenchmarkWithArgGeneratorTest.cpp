#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

class IncGenerator
{
public:
    typedef size_t ArgType;

    IncGenerator() {}

public:
    std::vector<size_t> Generate(int, char**)
    {
        return{ 1, 2, 3, 4, 5 };
    }
};

} // namespace

template<typename Generator>
using BM = sltbench::BenchmarkWithArgGenerator<Generator>;

TEST(BenchmarkWithArgGenerator, GetNameReturnsBenchmarkName)
{
    BM<IncGenerator> bm("name", [](const size_t&) {});
    EXPECT_EQ("name", bm.GetName());
}

TEST(BenchmarkWithArgGenerator, MeasureCallsFunction)
{
    bool called = false;
    BM<IncGenerator> bm("name", [&](const size_t&) { called = true;  });

    bm.Prepare();
    bm.Measure();

    EXPECT_TRUE(called);
}

TEST(BenchmarkWithArgGenerator, MeasureCallsFunctionWithGeneratedArg)
{
    std::vector<size_t> call_args;
    BM<IncGenerator> bm("name", [&](const size_t& arg) { call_args.push_back(arg);  });

    bm.Prepare();
    bm.Measure();
    bm.OnArgProcessed();
    bm.Measure();
    bm.OnArgProcessed();
    bm.Measure();
    bm.OnArgProcessed();

    ASSERT_EQ(3, call_args.size());
    EXPECT_EQ(1, call_args[0]);
    EXPECT_EQ(2, call_args[1]);
    EXPECT_EQ(3, call_args[2]);
}

TEST(BenchmarkWithArgGenerator, CurrentArgAsString)
{
    BM<IncGenerator> bm("name", [&](const size_t&) {});

    bm.Prepare();

    EXPECT_EQ("1", bm.CurrentArgAsString());
}

TEST(BenchmarkWithArgGenerator, HasArgsToProcessReturnsTrueAfterPrepare)
{
    BM<IncGenerator> bm("name", [&](const size_t&) {});

    bm.Prepare();

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithArgGenerator, HasArgsToProcessReturnsTrueAfterThreeArgsProcessed)
{
    BM<IncGenerator> bm("name", [&](const size_t&) {});

    bm.Prepare();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithArgGenerator, HasArgsToProcessReturnsTrueAfterAllArgsProcessed)
{
    BM<IncGenerator> bm("name", [&](const size_t&) {});

    bm.Prepare();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();

    EXPECT_FALSE(bm.HasArgsToProcess());
}
