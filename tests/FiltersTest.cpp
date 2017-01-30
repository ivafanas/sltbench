#include "gtest/gtest.h"

#include "src/Filters.h"


TEST(NullFilter, ShouldNotFilter)
{
	sltbench::NullFilter filter;

	EXPECT_TRUE(filter.ShouldRunBenchmark(""));
	EXPECT_TRUE(filter.ShouldRunBenchmark("benchmark"));
	EXPECT_TRUE(filter.ShouldRunBenchmark("12345"));
}

TEST(RegexFilter, ShouldFilterByBasicRegex)
{
	sltbench::RegexFilter filter(std::regex(".*mybench.*"));

	EXPECT_TRUE(filter.ShouldRunBenchmark("mybench"));
	EXPECT_TRUE(filter.ShouldRunBenchmark("Hoho_mybench"));
	EXPECT_TRUE(filter.ShouldRunBenchmark("mybench_hoho"));
	EXPECT_TRUE(filter.ShouldRunBenchmark("hoho_mybench_hoho"));

	EXPECT_FALSE(filter.ShouldRunBenchmark("myben"));
	EXPECT_FALSE(filter.ShouldRunBenchmark("magic!"));
}
