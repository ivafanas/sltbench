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
using BM = sltbench::Benchmark_F<Fixture>;

TEST(Benchmark_F, GetNameShouldReturnBenchmarkName)
{
	BM<Fixture> bm("name", &stub_func);

	EXPECT_EQ("name", bm.GetName());
}

TEST(Benchmark_F, MeasureCallsFunction)
{
	g_calls_count = 0;
	BM<Fixture> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_F, MeasureCallsFunctionExactlyOnce)
{
	g_calls_count = 0;
	BM<Fixture> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_F, MeasureCallsFunctionWithFixtureValue)
{
	g_arg = 0;
	BM<Fixture> bm("name", &set_arg);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(42, g_arg);
}

TEST(Benchmark_F, HasArgsToProcessReturnsTrueAfterPrepare)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(Benchmark_F, HasArgsToProcessReturnsFalseAfterArgProcessed)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();
	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(Benchmark_F, CurrentArgAsString)
{
	BM<Fixture> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_TRUE(bm.CurrentArgAsString().empty());
}
