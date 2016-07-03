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

    algo.SetFirstTimingResult(seconds(1));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(milliseconds(990));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(milliseconds(500));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(milliseconds(1010));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(990), res);
}

TEST(MeasureAlgo, DoesNotReturnsResultUntilMinExecutionTimeAchieved)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    algo.SetFirstTimingResult(milliseconds(25));
    EXPECT_TRUE(algo.NeedMoreTiming());

    for (size_t i = 1; i < 6; ++i)
    {
        algo.AddTimingResult(milliseconds(25));
        EXPECT_TRUE(algo.NeedMoreTiming());
    }

    algo.AddTimingResult(seconds(3));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(25), res);
}

TEST(MeasureAlgo, ReturnsBestTimeAfterMaxExecutionTimeAchieved)
{
    auto conf = MakeConf();
    sltbench::MeasureAlgo algo{conf};

    algo.SetFirstTimingResult(milliseconds(25));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(milliseconds(10));
    EXPECT_TRUE(algo.NeedMoreTiming());

    algo.AddTimingResult(minutes(3));
    EXPECT_FALSE(algo.NeedMoreTiming());

    auto res = algo.GetResult();
    EXPECT_EQ(milliseconds(10), res);
}
