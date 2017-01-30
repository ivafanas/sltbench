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
		return{generated_count_ * 100000, generated_count_};
	}

private:
	size_t generated_count_ = 0;
};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{
	return os << rhs.size << '/' << rhs.ncalls;
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

void ShuffleWithLazyGenerator(Fixture::Type& fix, const Generator::ArgType& arg)
{
	// some useful work here based on fixture and arg
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(fix.begin(), fix.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(ShuffleWithLazyGenerator, Fixture, Generator);

} // namespace
