#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <vector>


namespace {

class Generator
{
public:
	struct ArgType
	{
		size_t size;
		size_t ncalls;
	};

	Generator() {}

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
		return{{100000, 10}, {200000, 20}, {300000, 30}};
	}
};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{
	return os << rhs.ncalls << '/' << rhs.size;
}

class Fixture
{
public:
	typedef std::vector<size_t> Type;

	Fixture() {}

	Type& SetUp(const Generator::ArgType& arg)
	{
		fixture_.resize(arg.size, 0);
		for (size_t i = 0; i < arg.size; ++i)
			fixture_[i] = i;
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

void ShuffleWithGenerator(Fixture::Type& fix, const Generator::ArgType& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(fix.begin(), fix.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(ShuffleWithGenerator, Fixture, Generator);

} // namespace
