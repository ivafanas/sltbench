#include "gtest/gtest.h"

#include "src/MeasureAlgo.h"


using namespace std::chrono;


static sltbench::MeasureAlgo::Conf MakeConf()
{
    sltbench::MeasureAlgo::Conf rv;
    rv.precision_percents = 5;
    rv.max_execution_time = minutes(1);
    rv.min_execution_time = milliseconds(250);
    rv.dot_params = {
        { seconds(3), 1 },
        { milliseconds(100), 3 },
        { milliseconds(50),  4 },
        { milliseconds(20),  5 },
        { milliseconds(10),  10 },
        { microseconds(250), 15 },
        { microseconds(100), 18 },
        { nanoseconds::zero(), 20 }
    };
    return rv;
}

static sltbench::single_measure_algo::SingleMeasureResult MakeOneCallSMR(nanoseconds res_time)
{
	sltbench::single_measure_algo::SingleMeasureResult rv;
	rv.result = res_time;
	rv.total_time = res_time;
	return rv;
}

static sltbench::single_measure_algo::EstimationResult MakeOneCallER(nanoseconds res_time)
{
	sltbench::single_measure_algo::EstimationResult rv;
	rv.result = res_time;
	rv.total_time = res_time;
	rv.recommended_calls_count = 1;
	return rv;
}

TEST(MeasureAlgo, EmptyDatasetRequiresMoreTiming)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    EXPECT_TRUE(algo.NeedMoreTiming());
}

TEST(MeasureAlgo, DOTParamRequiredThreeMeasuresShoulFinishAfterThreeMeasures)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    algo.SetEstimationResult(MakeOneCallER(seconds(1)));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(MakeOneCallSMR(milliseconds(990)));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(MakeOneCallSMR(milliseconds(500)));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(MakeOneCallSMR(milliseconds(1010)));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(990), res);
}

TEST(MeasureAlgo, DoesNotReturnsResultUntilMinExecutionTimeAchieved)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    algo.SetEstimationResult(MakeOneCallER(milliseconds(25)));
    EXPECT_TRUE(algo.NeedMoreTiming());

    for (size_t i = 1; i < 6; ++i)
    {
        algo.AddTimingResult(MakeOneCallSMR(milliseconds(25)));
        EXPECT_TRUE(algo.NeedMoreTiming());
    }

    algo.AddTimingResult(MakeOneCallSMR(seconds(3)));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(25), res);
}

TEST(MeasureAlgo, ReturnsBestTimeAfterMaxExecutionTimeAchieved)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    algo.SetEstimationResult(MakeOneCallER(milliseconds(25)));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(MakeOneCallSMR(milliseconds(10)));
    EXPECT_TRUE(algo.NeedMoreTiming());

	algo.AddTimingResult(MakeOneCallSMR(minutes(3)));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(10), res);
}
