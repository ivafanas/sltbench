#include <sltbench/Bench.h>

#include <algorithm>
#include <vector>


namespace {

class Fixture
{
public:
	typedef std::vector<size_t> Type;

	Fixture() {}

	Type& SetUp()
	{
		const size_t size = 100000;
		fixture_.resize(size, 0);
		for (size_t i = 0; i < size; ++i)
			fixture_[i] = size - i;
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};


void Sort(Fixture::Type& fix)
{
	std::sort(fix.begin(), fix.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE(Sort, Fixture);

} // namespace
