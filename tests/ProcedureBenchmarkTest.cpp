#include "gtest/gtest.h"

#include "src/ProcedureBenchmark.h"


namespace {

void stub_func() {}

size_t g_n_calls = 0;

void inc_n_calls()
{
	++g_n_calls;
}

} // namespace

TEST(ProcedureBenchmark, GetNameReturnsBenchmarkName)
{
	sltbench::ProcedureBenchmark bm("name", &stub_func);

	EXPECT_EQ("name", bm.GetName());
}

TEST(ProcedureBenchmark, HasArgsToProcessReturnsTrue)
{
	sltbench::ProcedureBenchmark bm("name", &stub_func);

	EXPECT_TRUE(bm.HasArgsToProcess());
}

TEST(ProcedureBenchmark, HasArgsToProcessReturnsFalseAfterProcess)
{
	sltbench::ProcedureBenchmark bm("name", &stub_func);

	bm.OnArgProcessed();

	EXPECT_FALSE(bm.HasArgsToProcess());
}

TEST(ProcedureBenchmark, ConvertArgToStringReturnsEmptyString)
{
	sltbench::ProcedureBenchmark bm("name", &stub_func);

	EXPECT_TRUE(bm.CurrentArgAsString().empty());
}

TEST(ProcedureBenchmark, MeasureShouldCallFunctionOnce)
{
	g_n_calls = 0;
	sltbench::ProcedureBenchmark bm("name", &inc_n_calls);

	bm.Measure(1u);

	EXPECT_EQ(1u, g_n_calls);
}

TEST(ProcedureBenchmark, MeasureShouldCallFunctionRequiredTimes)
{
	g_n_calls = 0;
	sltbench::ProcedureBenchmark bm("name", &inc_n_calls);

	bm.Measure(3u);

	EXPECT_EQ(3u, g_n_calls);
}
