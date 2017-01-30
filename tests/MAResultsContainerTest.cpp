#include "gtest/gtest.h"

#include "src/MAResultsContainer.h"


using namespace sltbench;


TEST(MAResultsContainer, GetBestIsZeroForEmptyContainer)
{
	MAResultsContainer cont;
	EXPECT_EQ(0u, cont.GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForSingleResultContainer)
{
	MAResultsContainer cont;
	cont.Add(50);
	EXPECT_EQ(50u, cont.GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForSingleElementContainer)
{
	MAResultsContainer cont;
	cont.Add(50);
	cont.Add(50);
	EXPECT_EQ(50u, cont.GetBest());
}

TEST(MAResultsContainer, GetBestIsMinimumForMultiElementsContainer)
{
	MAResultsContainer cont;
	cont.Add(50);
	cont.Add(55);
	cont.Add(60);
	EXPECT_EQ(50u, cont.GetBest());
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForEmptyContainer)
{
	MAResultsContainer cont;
	EXPECT_EQ(0u, cont.GetMinSpotValue(1, 1));
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForContainerSizeLessThanSpot)
{
	MAResultsContainer cont;
	cont.Add(50);
	cont.Add(50);
	EXPECT_EQ(0u, cont.GetMinSpotValue(3, 1));
}

TEST(MAResultsContainer, GetMinSpotValueIsZeroForNoSuchSpot)
{
	MAResultsContainer cont;
	cont.Add(50);
	cont.Add(50);
	cont.Add(60);
	cont.Add(60);
	cont.Add(70);
	EXPECT_EQ(0u, cont.GetMinSpotValue(3, 1));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForSingleElementSpot)
{
	MAResultsContainer cont;
	cont.Add(50);
	cont.Add(50);
	EXPECT_EQ(50u, cont.GetMinSpotValue(2, 1));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForMultipleUniresults)
{
	MAResultsContainer cont;
	cont.Add(900);
	cont.Add(1000);
	cont.Add(1020);
	cont.Add(1100);
	EXPECT_EQ(1000, cont.GetMinSpotValue(2, 3));
}

TEST(MAResultsContainer, GetMinSpotValueCorrectForMultiElementSpot)
{
	MAResultsContainer cont;
	cont.Add(900);
	cont.Add(1000);
	cont.Add(1003);
	cont.Add(1020);
	cont.Add(1100);
	EXPECT_EQ(1000, cont.GetMinSpotValue(3, 3));
}
