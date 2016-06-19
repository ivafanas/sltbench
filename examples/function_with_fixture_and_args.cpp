#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <vector>



struct NSortFunctionArg
{
	size_t size;
	size_t ncalls;
};

std::ostream& operator << (std::ostream& oss, const NSortFunctionArg& arg)
{
	return oss << arg.size << '/' << arg.ncalls;
}

class NSortFunctionFixture
{
public:
	typedef std::vector<size_t> Type;

	NSortFunctionFixture() {}	
	
	Type& SetUp(const NSortFunctionArg& arg)
	{
		const size_t size = arg.size;
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

void NSortFunction(NSortFunctionFixture::Type& fixture, const NSortFunctionArg& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
	{
		std::sort(fixture.begin(), fixture.end());
	}
}
static const std::vector<NSortFunctionArg> nsortfunctions_args = {
	{ 100000, 5 },
	{ 110000, 4 },
	{ 120000, 3 }
};
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(NSortFunction, NSortFunctionFixture, nsortfunctions_args);
