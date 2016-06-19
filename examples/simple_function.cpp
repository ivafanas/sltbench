#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


void SimpleFunction()
{
	const size_t size = 100000;
	std::vector<size_t> v(size, 0);
	for (size_t i = 0; i < size; ++i)
	{
		v[i] = size - i;
	}
	std::sort(v.begin(), v.end());
}
SLTBENCH_FUNCTION(SimpleFunction);
