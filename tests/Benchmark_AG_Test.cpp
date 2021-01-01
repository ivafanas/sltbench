#include "gtest/gtest.h"

#include "TestAlgorithms.h"

#include <sltbench/Bench.h>


using namespace sltbench;


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

void stub_func(const size_t&)
{
}

size_t g_calls_count = 0;

void inc_calls_count(const size_t&)
{
	++g_calls_count;
}

} // namespace

template<typename Generator>
using BM = Benchmark_AG<Generator>;

TEST(Benchmark_AG, GetNameReturnsBenchmarkName)
{
	BM<IncGenerator> bm("name", &stub_func);
	EXPECT_EQ("name", std::string(bm.name));
}

TEST(Benchmark_AG, MeasureCallsFunction)
{
	g_calls_count = 0;
	BM<IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_AG, MeasureCallsRequiredCount)
{
	g_calls_count = 0;
	BM<IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(3u, g_calls_count);
}

TEST(Benchmark_AG, MeasureCallsFunctionRequiredCountOfTimes)
{
	g_calls_count = 0;
	BM<IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(3u, g_calls_count);
}

namespace {

std::vector<size_t> g_call_args;

void add_to_call_args(const size_t& n)
{
	g_call_args.push_back(n);
}


} // namespace

TEST(Benchmark_AG, MeasureCallsFunctionWithGeneratedArg)
{
	g_call_args.clear();
	BM<IncGenerator> bm("name", &add_to_call_args);

	bm.Prepare();
	bm.Measure(1u);
	bm.OnArgProcessed();
	bm.Measure(1u);
	bm.OnArgProcessed();
	bm.Measure(1u);
	bm.OnArgProcessed();

	EXPECT_TRUE(test_sequence_is(g_call_args, {1, 2, 3}));
}

TEST(Benchmark_AG, CurrentArgAsString)
{
	BM<IncGenerator> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_EQ("1", bm.CurrentArgAsString());
}

TEST(Benchmark_AG, HasArgsToProcessReturnsTrueAfterPrepare)
{
	BM<IncGenerator> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(Benchmark_AG, HasArgsToProcessReturnsTrueAfterThreeArgsProcessed)
{
	BM<IncGenerator> bm("name", &stub_func);

	bm.Prepare();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(Benchmark_AG, HasArgsToProcessReturnsTrueAfterAllArgsProcessed)
{
	BM<IncGenerator> bm("name", &stub_func);

	bm.Prepare();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();
	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}
