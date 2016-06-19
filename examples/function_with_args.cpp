#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>


void SortFunctionWithArg(const size_t& arg)
{
	std::vector<size_t> vec(arg, 0);
	for (size_t i = 0; i < vec.size(); ++i)
	{
		vec[i] = vec.size() - i;
	}
	std::sort(vec.begin(), vec.end());
}
static const std::vector<size_t> sort_function_args{ 100000, 110000, 120000, 130000, 140000, 150000 };
SLTBENCH_FUNCTION_WITH_ARGS(SortFunctionWithArg, sort_function_args);



struct StringMultFunctionArg
{
	size_t n;
	std::string src;
};

std::ostream& operator << (std::ostream& oss, const StringMultFunctionArg& rhs)
{
	return oss << rhs.n << '/' << rhs.src;
}

void StringMultFunction(const StringMultFunctionArg& arg)
{
	std::string rv;
	for (size_t i = 0; i < arg.n; ++i)
		rv += arg.src;
}

static std::vector<StringMultFunctionArg> string_mult_args{
	{ 100000, "do not worry" },
	{ 200000, "be happy" }
};

SLTBENCH_FUNCTION_WITH_ARGS(StringMultFunction, string_mult_args);
