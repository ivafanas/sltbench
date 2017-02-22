#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


namespace {

std::vector<size_t> MakeSortFixture(const size_t& size)
{
	std::vector<size_t> rv(size, 0);
	for (size_t i = 0; i < size; ++i)
		rv[i] = size - i;
	return rv;
}

void Sort_FBA(std::vector<size_t>& fix, const size_t& arg)
{
	// code to benchmark using fix and arg
	std::sort(fix.begin(), fix.end());
}

const std::vector<size_t> args = { 1024, 2048, 4096 };

SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(Sort_FBA, MakeSortFixture, args);

} // namespace
