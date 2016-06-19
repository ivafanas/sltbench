#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


class SortFunctionFixture
{
public:
	typedef std::vector<size_t> Type;

	SortFunctionFixture() {}	
	
	Type& SetUp()
	{
		const size_t size = 100000;
		fixture_.resize(size, 0);
		for (size_t i = 0; i < size; ++i)
		{
			fixture_[i] = size - i;
		}
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};


void SortFunction(SortFunctionFixture::Type& fixture)
{
	std::sort(fixture.begin(), fixture.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE(SortFunction, SortFunctionFixture);
