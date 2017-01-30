#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <vector>


namespace {

struct Arg
{
	size_t size;
	size_t ncalls;
};

std::ostream& operator << (std::ostream& oss, const Arg& arg)
{
	return oss << arg.size << '/' << arg.ncalls;
}

class Fixture
{
public:
	typedef std::vector<size_t> Type;

	Fixture() {}

	Type& SetUp(const Arg& arg)
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

void Shuffle(Fixture::Type& fix, const Arg& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(fix.begin(), fix.end());
}
static const std::vector<Arg> args = {
	{ 1, 1 },
	{ 2, 1 },
	{ 100000, 1 },
	{ 200000, 2 },
	{ 300000, 3 }
};

SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(Shuffle, Fixture, args);

} // namespace
