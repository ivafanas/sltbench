#include <sltbench/Bench.h>


void SimpleFunction_SHOULD_THROW()
{
	throw std::runtime_error("hmm, something is wrong");
}
SLTBENCH_FUNCTION(SimpleFunction_SHOULD_THROW);
