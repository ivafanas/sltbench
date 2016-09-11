#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


void SimpleFunction()
{
    std::string rv;
    for (size_t i = 0; i < 100000; ++i)
        rv += "simple function";
}
SLTBENCH_FUNCTION(SimpleFunction);
