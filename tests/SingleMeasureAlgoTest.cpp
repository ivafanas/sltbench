#include "gtest/gtest.h"

#include "src/SingleMeasureAlgo.h"

#include <chrono>


using namespace sltbench;
using namespace std::chrono;


TEST(Estimate, OneNanosecondFunctionShouldBeOKIfMulticountEnabled)
{
	const auto fun = [](size_t) { return nanoseconds(1); };
	const bool is_multicount_enabled = true;

	const auto estimation = Estimate(fun, is_multicount_enabled);

	EXPECT_EQ(EstimationResult::Verdict::OK, estimation.verdict);
}

TEST(Estimate, OneNanosecondFunctionShouldBeImpreciseIfMulticountDisabled)
{
	const auto fun = [](size_t) { return nanoseconds(1); };
	const bool is_multicount_enabled = false;

	const auto estimation = Estimate(fun, is_multicount_enabled);

	EXPECT_EQ(EstimationResult::Verdict::CANNOT_BE_PRECISE, estimation.verdict);
	EXPECT_EQ(1u, estimation.recommended_calls_count);
}

TEST(Estimate, OneMillisecondFunctionShouldBeOK)
{
	const auto fun = [](size_t) -> nanoseconds { return milliseconds(1); };
	const bool is_multicount_enabled = false;

	const auto estimation = Estimate(fun, is_multicount_enabled);

	EXPECT_EQ(EstimationResult::Verdict::OK, estimation.verdict);
}

TEST(Estimate, OneSecondFunctionReturnValue)
{
	const auto fun = [](size_t) -> nanoseconds { return seconds(1); };
	const bool is_multicount_enabled = true;

	const auto estimation = Estimate(fun, is_multicount_enabled);

	EXPECT_EQ(seconds(1), estimation.result);
	EXPECT_EQ(seconds(1), estimation.total_time);
	EXPECT_EQ(1, estimation.recommended_calls_count);
}

TEST(Estimate, TenNanosecondsFunctionReturnValue)
{
	const auto fun = [](size_t) -> nanoseconds { return nanoseconds(10); };
	const bool is_multicount_enabled = true;

	const auto estimation = Estimate(fun, is_multicount_enabled);

	EXPECT_GT(estimation.total_time, nanoseconds(100));
	EXPECT_GT(estimation.recommended_calls_count, 10u);
}
