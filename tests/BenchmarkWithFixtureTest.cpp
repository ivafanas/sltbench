#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

class Fixture
{
public:
    typedef int Type;

    Fixture() {}

    int& SetUp()
    {
        fixture_ = 42;
        return fixture_;
    }

    void TearDown() {}

private:
    int fixture_;
};

} // namespace

template<typename Fixture>
using BM = sltbench::BenchmarkWithFixture<Fixture>;

TEST(BenchmarkWithFixture, GetNameShouldReturnBenchmarkName)
{
    BM<Fixture> bm("name", [](int&) {});

    EXPECT_EQ("name", bm.GetName());
}

TEST(BenchmarkWithFixture, MeasureCallsFunction)
{
    bool called = false;
    BM<Fixture> bm("name", [&](int&) { called = true;  });

    bm.Prepare();
    bm.Measure();

    EXPECT_TRUE(called);
}

TEST(BenchmarkWithFixture, MeasureCallsFunctionWithFixtureValue)
{
    int arg;
    BM<Fixture> bm("name", [&](int& fix) { arg = fix; });

    bm.Prepare();
    bm.Measure();

    EXPECT_EQ(42, arg);
}

TEST(BenchmarkWithFixture, HasArgsToProcessReturnsTrueAfterPrepare)
{
    BM<Fixture> bm("name", [](int& fix){});

    bm.Prepare();

    EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixture, HasArgsToProcessReturnsFalseAfterArgProcessed)
{
    BM<Fixture> bm("name", [](int& fix){});

    bm.Prepare();
    bm.OnArgProcessed();

    EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixture, CurrentArgAsString)
{
    BM<Fixture> bm("name", [](int& fix){});

    bm.Prepare();

    EXPECT_TRUE(bm.CurrentArgAsString().empty());
}
