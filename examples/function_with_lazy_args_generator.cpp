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

void ShuffleWithLazyGenerator(const Generator::ArgType& arg)
{
	// let's propose we are going to do something useful computations here
	std::vector<size_t> vec(arg.size, 0);
	for (size_t i = 0; i < arg.ncalls; ++i)
		std::random_shuffle(vec.begin(), vec.end());
}
SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(ShuffleWithLazyGenerator, Generator);

} // namespace
