#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <vector>


class SortFunctionArgsGenerator
{
public:
	struct SortFunctionArg
	{
		size_t count;
		size_t n_sorts;
	};

	typedef SortFunctionArg ArgType;

	SortFunctionArgsGenerator() {}

	std::vector<ArgType> Generate(int argc, char **argv)
	{
		// You may get input filename from argc,argv and
		// read your custom file and fill arguments vector.
		//
		// Or arguments might be passed via command line arguments, it is up to You.
		//
		// However, if the input dataset is known at compile-time,
		// you would better use SLTBENCH_FUNCTION_WITH_ARGS macro
		//
		// I will just left hardcoded solution here
		return { { 100000, 3 }, { 110000, 4 }, { 120000, 5 } };
	}
};

std::ostream& operator << (std::ostream& os, const SortFunctionArgsGenerator::SortFunctionArg& rhs)
{
	return os << rhs.count << '/' << rhs.n_sorts;
}

void SortFunctionWithArgGenerator(const SortFunctionArgsGenerator::ArgType& arg)
{
	// let's propose we are going to do something useful computations here
	std::vector<size_t> vec(arg.count, 0);
	for (size_t i = 0; i < vec.size(); ++i)
	{
		vec[i] = vec.size() - i;
	}
	for (size_t i = 0; i < arg.n_sorts; ++i)
	{
		std::sort(vec.begin(), vec.end());
	}
}
SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(SortFunctionWithArgGenerator, SortFunctionArgsGenerator);
