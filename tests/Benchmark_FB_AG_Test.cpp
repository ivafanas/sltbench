#include "gtest/gtest.h"

#include <sltbench/Bench.h>


namespace {

size_t g_fixtures_made_count = 0;

size_t g_make_fixture_arg = 0;

int make_fixture(const size_t& arg)
{
	++g_fixtures_made_count;
	g_make_fixture_arg = arg;
	return 42;
}

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

void stub_func(int&, const size_t&) {}

size_t g_calls_count = false;

void inc_calls_count(int&, const size_t&)
{
	++g_calls_count;
}

int g_fix = 0;
size_t g_arg = 0;

void set_fix_arg(int& fix, const size_t& arg)
{
	g_fix = fix;
	g_arg = arg;
}

} // namespace

template<typename Fixture, typename Generator>
using BM = sltbench::Benchmark_FB_AG<Fixture, Generator>;

TEST(Benchmark_FB_AG, GetNameShouldReturnBenchmarkName)
{
	BM<int, IncGenerator> bm("name", &stub_func, &make_fixture);

	EXPECT_EQ("name", bm.GetName());
}

TEST(Benchmark_FB_AG, MeasureCallsFunction)
{
	g_calls_count = 0;
	g_fixtures_made_count = 0;
	BM<int, IncGenerator> bm("name", &inc_calls_count, &make_fixture);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
	EXPECT_EQ(1u, g_fixtures_made_count);
}

TEST(Benchmark_FB_AG, MeasureCallsFunctionExactlyOnce)
{
	g_calls_count = 0;
	g_fixtures_made_count = 0;
	BM<int, IncGenerator> bm("name", &inc_calls_count, &make_fixture);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(1u, g_calls_count);
	EXPECT_EQ(1u, g_fixtures_made_count);
}

TEST(Benchmark_FB_AG, MeasureCallsFunctionWithFixtureValue)
{
	g_fix = 0;
	g_arg = 0;
	BM<int, IncGenerator> bm("name", &set_fix_arg, &make_fixture);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1, g_arg);
	EXPECT_EQ(42, g_fix);
}

TEST(Benchmark_FB_AG, HasArgsToProcessReturnsTrueAfterPrepare)
{
	BM<int, IncGenerator> bm("name", &stub_func, &make_fixture);

	bm.Prepare();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(Benchmark_FB_AG, HasArgsToProcessReturnsFalseAfterAllArgProcessed)
{
	BM<int, IncGenerator> bm("name", &stub_func, &make_fixture);

	bm.Prepare();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}
