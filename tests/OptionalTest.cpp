#include "gtest/gtest.h"

#include <sltbench/impl/Optional.h>


using namespace sltbench;


namespace
{
	class DtorCounter
	{
	public:
		DtorCounter(int& counter) : counter_(counter) {}
		~DtorCounter() { ++counter_; }

	private:
		int& counter_;
	};
}

TEST(Optional, IsInitializedEmpty)
{
	scoped_optional<int> x;
	EXPECT_FALSE(x.is_initialized());
}

TEST(Optional, IsInitializedNonEmpty)
{
	scoped_optional<int> x;
	x.emplace(3);
	EXPECT_TRUE(x.is_initialized());
}

TEST(Optional, ResetOnEmpty)
{
	scoped_optional<int> x;
	x.reset();
	EXPECT_FALSE(x.is_initialized());
}

TEST(Optional, ResetOnNonEmpty)
{
	scoped_optional<int> x;
	x.emplace(3);
	x.reset();
	EXPECT_FALSE(x.is_initialized());
}

TEST(Optional, DtorCallsSubobjectDtor)
{
	int count = 0;
	{
		scoped_optional<DtorCounter> x;
		x.emplace(count);
	}
	EXPECT_EQ(count, 1);
}

TEST(Optional, ResetCallsSubobjectDtor)
{
	int count = 0;
	scoped_optional<DtorCounter> x;
	x.emplace(count);
	x.reset();
	EXPECT_EQ(count, 1);
}

TEST(Optional, EmplaceCallsSubobjectDtor)
{
	int count = 0;
	scoped_optional<DtorCounter> x;
	x.emplace(count);
	x.emplace(count);
	EXPECT_EQ(count, 1);
}

TEST(Optional, EmplaceOverwritesObject)
{
	scoped_optional<int> x;
	x.emplace(1);
	x.emplace(2);
	EXPECT_EQ(x.get(), 2);
}

TEST(Optional, GetPtrOnEmpty)
{
	scoped_optional<int> x;
	EXPECT_EQ(x.get_ptr(), nullptr);
}

TEST(Optional, GetPtrOnNonEmpty)
{
	scoped_optional<int> x;
	x.emplace(1);
	EXPECT_NE(x.get_ptr(), nullptr);
	EXPECT_EQ(*x.get_ptr(), 1);
}

