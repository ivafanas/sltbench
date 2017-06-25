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
		std::string src;
		size_t n;
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
		// you would better use SLTBENCH_FUNCTION_WITH_ARGS macro
		//
		// I will just left hardcoded solution here
		return{{"knowledge", 100000}, {"power", 200000}};
	}
};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{
	return os << rhs.n << '/' << rhs.src;
}

void StringMultWithGenerator(const Generator::ArgType& arg)
{
	std::string rv;
	for (size_t i = 0; i < arg.n; ++i)
		rv += arg.src;
}
SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(StringMultWithGenerator, Generator);

} // namespace
