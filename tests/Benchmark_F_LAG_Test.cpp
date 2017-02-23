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

void stub_func(int&, const size_t&) {}

size_t g_calls_count = 0;

void inc_calls_count(int&, const size_t&)
{
	++g_calls_count;
}

std::vector<size_t> g_call_args;
std::vector<size_t> g_call_fixs;

void push_back_arg_and_fix(int& fix, const size_t& arg)
{
	g_call_args.push_back(arg);
	g_call_fixs.push_back(fix);
}

} // namespace

template<typename Fixture, typename Generator>
using BM = sltbench::Benchmark_F_LAG<Fixture, Generator>;

TEST(Benchmark_F_LAG, GetNameReturnsBenchmarkName)
{
	BM<Fixture, IncGenerator> bm("name", &stub_func);
	EXPECT_EQ("name", bm.GetName());
}

TEST(Benchmark_F_LAG, MeasureCallsFunction)
{
	g_calls_count = 0;
	BM<Fixture, IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(1u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_F_LAG, MeasureCallsFunctionExactlyOnce)
{
	g_calls_count = 0;
	BM<Fixture, IncGenerator> bm("name", &inc_calls_count);

	bm.Prepare();
	bm.Measure(3u);

	EXPECT_EQ(1u, g_calls_count);
}

TEST(Benchmark_F_LAG, MeasureCallsFunctionWithGeneratedArg)
{
	g_call_args.clear();
	g_call_fixs.clear();
	BM<Fixture, IncGenerator> bm("name", &push_back_arg_and_fix);

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

	ASSERT_EQ(3, g_call_fixs.size());
	EXPECT_EQ(5, g_call_fixs[0]);
	EXPECT_EQ(5, g_call_fixs[1]);
	EXPECT_EQ(5, g_call_fixs[2]);
}

TEST(Benchmark_F_LAG, CurrentArgAsString)
{
	BM<Fixture, IncGenerator> bm("name", &stub_func);

	bm.Prepare();

	EXPECT_EQ("1", bm.CurrentArgAsString());
}
