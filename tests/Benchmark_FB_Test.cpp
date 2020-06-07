#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

size_t g_fixtures_made_count = 0;

int make_fixture()
{
	++g_fixtures_made_count;
	return 42;
}

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
using BM = sltbench::Benchmark_FB<Fixture>;

TEST(Benchmark_FB, GetNameShouldReturnBenchmarkName)
{
	BM<int> bm("name", &stub_func, &make_fixture);

	EXPECT_EQ("name", std::string(bm.name));
}

TEST(Benchmark_FB, MeasureCallsFunction)
{
	g_calls_count = 0;
	g_fixtures_made_count = 0;
	BM<int> bm("name", &inc_calls_count, &make_fixture);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
	EXPECT_EQ(1u, g_fixtures_made_count);
}

TEST(Benchmark_FB, MeasureCallsFunctionExactlyOnce)
{
	g_calls_count = 0;
	g_fixtures_made_count = 0;
	BM<int> bm("name", &inc_calls_count, &make_fixture);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(1u, g_calls_count);
	EXPECT_EQ(1u, g_fixtures_made_count);
}

TEST(Benchmark_FB, MeasureCallsFunctionWithFixtureValue)
{
	g_arg = 0;
	BM<int> bm("name", &set_arg, &make_fixture);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(42, g_arg);
}

TEST(Benchmark_FB, HasArgsToProcessReturnsTrueAfterPrepare)
{
	BM<int> bm("name", &stub_func, &make_fixture);

	bm.Prepare();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(Benchmark_FB, HasArgsToProcessReturnsFalseAfterArgProcessed)
{
	BM<int> bm("name", &stub_func, &make_fixture);

	bm.Prepare();
	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(Benchmark_FB, CurrentArgAsString)
{
	BM<int> bm("name", &stub_func, &make_fixture);

	bm.Prepare();

	EXPECT_TRUE(bm.CurrentArgAsString().empty());
}
