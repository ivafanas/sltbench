#include "gtest/gtest.h"

#include "src/Runner.h"


class FakeRunner : public sltbench::IRunner
{
public:
    FakeRunner() = default;

public:
    virtual bool Run(
        sltbench::reporter::IReporter&,
        sltbench::IFilter&) override
    {
        return true;
    }
};

TEST(Runner, RegisterRunnerShouldSaveRunnerInContainer)
{
    const auto runner1 = std::make_shared<FakeRunner>();
    sltbench::RegisterRunner("runner1", runner1);

    const auto runner2 = std::make_shared<FakeRunner>();
    sltbench::RegisterRunner("runner2", runner2);

    const auto runners = sltbench::GetRunners();

    ASSERT_EQ(2, runners.size());
    if (runners[0] == runner1)
    {
        EXPECT_EQ(runner2, runners[1]);
    }
    else
    {
        EXPECT_EQ(runner2, runners[0]);
        EXPECT_EQ(runner1, runners[1]);
    }
}
