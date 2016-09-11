#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

class Fixture
{
public:
    typedef int Type;

    Fixture() {}

    int& SetUp(const size_t&)
    {
        fixture_ = 5;
        return fixture_;
    }

    void TearDown() {}

private:
    int fixture_;
};

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

template<typename Fixture, typename Generator>
using BM = sltbench::BenchmarkWithFixtureAndArgGenerator<Fixture, Generator>;

TEST(BenchmarkWithFixtureAndArgGenerator, GetNameShouldReturnBenchmarkName)
{
    BM<Fixture, IncGenerator> bm("name", [](int&, const size_t&) {});

    EXPECT_EQ("name", bm.GetName());
}

TEST(BenchmarkWithFixtureAndArgGenerator, MeasureCallsFunction)
{
    bool called = false;
    BM<Fixture, IncGenerator> bm("name", [&](int&, const size_t&) { called = true; });

    bm.Prepare();
    bm.Measure();

    EXPECT_TRUE(called);
}

TEST(BenchmarkWithFixtureAndArgGenerator, MeasureCallsFunctionWithFixtureAndArg)
{
    std::vector<size_t> call_args;
    std::vector<size_t> call_fixs;
    BM<Fixture, IncGenerator> bm("name", [&](int& fix, const size_t& arg) {
        call_fixs.push_back(fix);
        call_args.push_back(arg);
    });

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

    ASSERT_EQ(3, call_fixs.size());
    EXPECT_EQ(5, call_fixs[0]);
    EXPECT_EQ(5, call_fixs[1]);
    EXPECT_EQ(5, call_fixs[2]);
}

TEST(BenchmarkWithFixtureAndArgGenerator, CurrentArgAsString)
{
    BM<Fixture, IncGenerator> bm("name", [](int&, const size_t&) {});

    bm.Prepare();

    EXPECT_EQ("1", bm.CurrentArgAsString());
}

TEST(BenchmarkWithFixtureAndArgGenerator, HasArgsToProcessReturnsTrueAfterPrepare)
{
    BM<Fixture, IncGenerator> bm("name", [](int&, const size_t&) {});

    bm.Prepare();

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixtureAndArgGenerator, HasArgsToProcessReturnsTrueAfterThreeArgsProcessed)
{
    BM<Fixture, IncGenerator> bm("name", [](int&, const size_t&) {});

    bm.Prepare();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixtureAndArgGenerator, HasArgsToProcessReturnsTrueAfterAllArgsProcessed)
{
    BM<Fixture, IncGenerator> bm("name", [](int&, const size_t&) {});

    bm.Prepare();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();
    bm.OnArgProcessed();

    EXPECT_FALSE(bm.HasArgsToProcess());
}
