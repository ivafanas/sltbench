#include <sltbench/Bench.h>

#include <algorithm>
#include <ostream>
#include <set>
#include <sstream>
#include <vector>


namespace {

void InsertToSetWithArg(const size_t& arg)
{
	std::set<size_t> s;
	for (size_t i = 0; i < arg; ++i)
		s.insert(i);
}
const std::vector<size_t> sort_args{ 1, 2, 3, 4, 5, 100000, 110000, 120000, 130000, 140000, 150000 };
SLTBENCH_FUNCTION_WITH_ARGS(InsertToSetWithArg, sort_args);



struct StringMultArg
{
	size_t n;
	std::string src;
};

std::ostream& operator << (std::ostream& oss, const StringMultArg& rhs)
{
	return oss << rhs.n << '/' << rhs.src;
}

void StringMult(const StringMultArg& arg)
{
	std::string rv;
	for (size_t i = 0; i < arg.n; ++i)
		rv += arg.src;
}

const std::vector<StringMultArg> string_mult_args{
	{ 100000, "do not worry" },
	{ 200000, "be happy" }
};

SLTBENCH_FUNCTION_WITH_ARGS(StringMult, string_mult_args);

} // namespace
