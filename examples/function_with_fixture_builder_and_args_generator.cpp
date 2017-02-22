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
		// I will just left hardcoded solution here
		return{ { 100000, 10 },{ 200000, 20 },{ 300000, 30 } };
	}
};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{
	return os << rhs.ncalls << '/' << rhs.size;
}

std::vector<size_t> MakeFixture(const Generator::ArgType& arg)
{
	std::vector<size_t> rv(arg.size, 0);
	for (size_t i = 0; i < arg.size; ++i)
		rv[i] = arg.size - i;
	return rv;
}

void Shuffle_FBAG(std::vector<size_t>& fix, const Generator::ArgType& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(fix.begin(), fix.end());
}

SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(Shuffle_FBAG, MakeFixture, Generator);

} // namespace
