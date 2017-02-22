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

	Generator(int, char **) {}

	ArgType Generate()
	{
		if (generated_count_ >= 3)
			throw sltbench::StopGenerationException();

		++generated_count_;

		// the only instance of ArgType is in the memory during measurement
		return{ generated_count_ * 100000, generated_count_ };
	}

private:
	size_t generated_count_ = 0;
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

void Shuffle_FBLAG(std::vector<size_t>& fix, const Generator::ArgType& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(fix.begin(), fix.end());
}

SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(Shuffle_FBLAG, MakeFixture, Generator);

} // namespace
