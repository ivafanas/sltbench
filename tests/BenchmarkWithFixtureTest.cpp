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

void stub_func(int&) {}

size_t g_calls_count = false;

void inc_calls_count(int&)
{
	++g_calls_count;
}

int g_arg = 0;

void set_arg(int& arg)
{
	g_arg = arg;
}

} // namespace

template<typename Fixture>
using BM = sltbench::BenchmarkWithFixture<Fixture>;

TEST(BenchmarkWithFixture, GetNameShouldReturnBenchmarkName)
{
	BM<Fixture> bm("name", &stub_func);

	EXPECT_EQ("name", bm.GetName());
}

TEST(BenchmarkWithFixture, MeasureCallsFunction)
{
	g_calls_count = 0;
	BM<Fixture> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(BenchmarkWithFixture, MeasureCallsFunctionExactlyOnce)
{
	g_calls_count = 0;
	BM<Fixture> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(BenchmarkWithFixture, MeasureCallsFunctionWithFixtureValue)
{
	g_arg = 0;
	BM<Fixture> bm("name", &set_arg);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(42, g_arg);
}

TEST(BenchmarkWithFixture, HasArgsToProcessReturnsTrueAfterPrepare)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixture, HasArgsToProcessReturnsFalseAfterArgProcessed)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();
	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(BenchmarkWithFixture, CurrentArgAsString)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_TRUE(bm.CurrentArgAsString().empty());
}
