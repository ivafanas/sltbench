#include <sltbench/Bench.h>

#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <list>


//
// where insert is faster, to set or to unordered_set with reserve ?
//

static const std::vector<size_t> insert_to_set_args{ 1000, 10000, 100000, 1000000 };

static void InsertToSetSorted(const size_t& count)
{
	std::set< size_t > s;
	for (size_t i = 0; i < count; ++i)
		s.insert(i);
}
SLTBENCH_FUNCTION_WITH_ARGS(InsertToSetSorted, insert_to_set_args);

static void InsertToSetUnordered(const size_t& count)
{
	std::unordered_set<size_t> s;
	s.reserve(count);
	for (size_t i = 0; i < count; ++i)
		s.insert(i);
}
SLTBENCH_FUNCTION_WITH_ARGS(InsertToSetUnordered, insert_to_set_args);


//
// what about performance of std::accumulate for vector ?
//

static const std::vector<size_t> acc_args{ 1000, 10000, 100000, 1000000 };

class AccVectorFixture
{
public:
	typedef std::vector<double> Type;

	AccVectorFixture() {}

	Type& SetUp(const size_t& count)
	{
		if (fixture_.size() != count)
			fixture_.resize(count, 0.1);
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

static void AccVectorStd(AccVectorFixture::Type& fix, const size_t& count)
{
	auto res = std::accumulate(fix.begin(), fix.end(), 0.0);
	fix[0] = res;
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(AccVectorStd, AccVectorFixture, acc_args);

static void AccVectorNaive(AccVectorFixture::Type& fix, const size_t& count)
{
	double res = 0;
	for (auto v : fix)
		sltbench::DoNotOptimize(res += v);
	fix[0] = res;
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(AccVectorNaive, AccVectorFixture, acc_args);
