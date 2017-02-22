#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


namespace {

std::vector<size_t> MakeSortFixture()
{
	const size_t size = 100000;
	std::vector<size_t> rv(size, 0);
	for (size_t i = 0; i < size; ++i)
		rv[i] = size - i;
	return rv;
}

void Sort_FB(std::vector<size_t>& fix)
{
	// code to benchmark using fix
	std::sort(fix.begin(), fix.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(Sort_FB, MakeSortFixture);

} // namespace
