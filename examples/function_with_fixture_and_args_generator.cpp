#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <vector>


class NSortFunctionArgsGenerator
{
public:
	struct SortFunctionArg
	{
		size_t count;
		size_t ncalls;
	};

	typedef SortFunctionArg ArgType;

	NSortFunctionArgsGenerator() {}

	std::vector<ArgType> Generate(int argc, char **argv)
	{
		// You may get input filename from argc,argv and
		// read your custom file and fill arguments vector.
		//
		// Or arguments might be passed via command line arguments, it is up to You.
		//
		// However, if the input dataset is known at compile-time,
		// you would better use SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS macro
		//
		// I will just left hardcoded solution here
		return{ { 100000, 3 },{ 110000, 4 },{ 120000, 5 } };
	}
};

std::ostream& operator << (std::ostream& os, const NSortFunctionArgsGenerator::SortFunctionArg& rhs)
{
	return os << rhs.count << '/' << rhs.ncalls;
}

class NSortFunctionFixture
{
public:
	typedef std::vector<size_t> Type;

	NSortFunctionFixture() {}	
	
	Type& SetUp(const NSortFunctionArgsGenerator::ArgType& arg)
	{
		const size_t size = arg.count;
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

void NSortFunctionWGenerator(NSortFunctionFixture::Type& fixture, const NSortFunctionArgsGenerator::ArgType& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
	{
		std::sort(fixture.begin(), fixture.end());
	}
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(NSortFunctionWGenerator, NSortFunctionFixture, NSortFunctionArgsGenerator);
