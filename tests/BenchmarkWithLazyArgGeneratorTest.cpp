#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

class IncGenerator
{
public:
    typedef size_t ArgType;

    IncGenerator(int, char **) {}

public:
    size_t Generate()
    {
        ++i_;
        return i_;
    }

private:
    size_t i_ = 0;
};

} // namespace

template<typename Generator>
using BM = sltbench::BenchmarkWithLazyArgGenerator<Generator>;

TEST(BenchmarkWithLazyArgGenerator, GetNameReturnsBenchmarkName)
{
    BM<IncGenerator> bm("name", [](const size_t&) {});
    EXPECT_EQ("name", bm.GetName());
}

TEST(BenchmarkWithLazyArgGenerator, MeasureCallsFunction)
{
    bool called = false;
    BM<IncGenerator> bm("name", [&](const size_t&) { called = true;  });

    bm.Prepare();
    bm.Measure();

    EXPECT_TRUE(called);
}

TEST(BenchmarkWithLazyArgGenerator, MeasureCallsFunctionWithGeneratedArg)
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

TEST(BenchmarkWithLazyArgGenerator, CurrentArgAsString)
{
    BM<IncGenerator> bm("name", [&](const size_t&) {});

    bm.Prepare();

    EXPECT_EQ("1", bm.CurrentArgAsString());
}
