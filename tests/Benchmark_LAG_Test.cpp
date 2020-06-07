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

void stub_func(const size_t&) {}

size_t g_calls_count = 0;

void inc_calls_count(const size_t&)
{
	++g_calls_count;
}

std::vector<size_t> g_call_args;

void push_back_arg(const size_t& arg)
{
	g_call_args.push_back(arg);
}

} // namespace

template<typename Generator>
using BM = sltbench::Benchmark_LAG<Generator>;

TEST(Benchmark_LAG, GetNameReturnsBenchmarkName)
{
	BM<IncGenerator> bm("name", &stub_func);
	EXPECT_EQ("name", std::string(bm.name));
}

TEST(Benchmark_LAG, MeasureCallsFunction)
{
	g_calls_count = 0;
	BM<IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_LAG, MeasureCallsFunctionRequiredNumberOfTimes)
{
	g_calls_count = 0;
	BM<IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(3u, g_calls_count);
}

TEST(Benchmark_LAG, MeasureCallsFunctionWithGeneratedArg)
{
	g_call_args.clear();
	BM<IncGenerator> bm("name", &push_back_arg);

	bm.Prepare();
	bm.Measure(1u);
	bm.OnArgProcessed();
	bm.Measure(1u);
	bm.OnArgProcessed();
	bm.Measure(1u);
	bm.OnArgProcessed();

	ASSERT_EQ(3, g_call_args.size());
	EXPECT_EQ(1, g_call_args[0]);
	EXPECT_EQ(2, g_call_args[1]);
	EXPECT_EQ(3, g_call_args[2]);
}

TEST(Benchmark_LAG, CurrentArgAsString)
{
	BM<IncGenerator> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_EQ("1", bm.CurrentArgAsString());
}
