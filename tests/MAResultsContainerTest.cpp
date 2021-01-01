#include "gtest/gtest.h"

#include "src/MAResultsContainer.h"

#include <cstdint>
#include <initializer_list>


using namespace sltbench;

static MAResultsContainer MakeContainer(
	std::initializer_list<std::uint64_t> values)
{
	MAResultsContainer cont;
	for (const auto value : values)
		cont.Add(value);
	return cont;
}

TEST(MAResultsContainer, GetBestIsZeroForEmptyContainer)
{
	MAResultsContainer cont;
	EXPECT_EQ(0u, cont.GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForSingleResultContainer)
{
	EXPECT_EQ(50u, MakeContainer({ 50u }).GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForSingleElementContainer)
{
	EXPECT_EQ(50u, MakeContainer({ 50u, 50u }).GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForMultiElementsContainer)
{
	EXPECT_EQ(50u, MakeContainer({ 50u, 55u, 60u }).GetBest());
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForEmptyContainer)
{
	MAResultsContainer cont;
	EXPECT_EQ(0u, cont.GetMinSpotValue(1, 1));
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForContainerSizeLessThanSpot)
{
	EXPECT_EQ(0u, MakeContainer({ 50u, 50u }).GetMinSpotValue(3, 1));
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForNoSuchSpot)
{
	const auto cont = MakeContainer({ 50u, 50u, 60u, 60u, 70u });
	EXPECT_EQ(0u, cont.GetMinSpotValue(3, 1));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForSingleElementSpot)
{
	EXPECT_EQ(50u, MakeContainer({ 50u, 50u }).GetMinSpotValue(2, 1));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForMultipleUniresults)
{
	const auto cont = MakeContainer({ 900u, 1000u, 1020u, 1100u });
	EXPECT_EQ(1000u, cont.GetMinSpotValue(2, 3));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForMultiElementSpot)
{
	const auto cont = MakeContainer({ 900u, 1000u, 1003u, 1020u, 1100u  });
	EXPECT_EQ(1000u, cont.GetMinSpotValue(3, 3));
}

