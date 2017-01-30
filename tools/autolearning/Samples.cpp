#include <sltbench/Bench.h>

#include <algorithm>
#include <set>
#include <sstream>
#include <unordered_set>


//
// insert data to set
//

static const std::vector<size_t> insert_to_set_args{
	1000,
	10000,
	50000,
	100000,
	200000,
	300000,
	400000,
	500000,
	600000,
	700000,
	800000,
	900000,	
	1000000,
	2000000,
	3500000,
	5000000
};

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
// sort vector
//

static const std::vector<size_t> sort_vector_args{
	1000,
	10000,
	100000,
	1000000,
	3000000,
	5000000,
	7000000,
	9000000,
	10000000,
	20000000,
	30000000,
};

class SortVectorFixture
{
public:
	typedef std::vector<size_t> Type;

	SortVectorFixture() {}

	Type& SetUp(const size_t& count)
	{
		size_t max_value = count / 10;
		fixture_.resize(count, 0);
		for (size_t i = 0; i < count; ++i)
		{
			fixture_[i] = (count - i) % max_value;
		}
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

static void SortVector(SortVectorFixture::Type& fixture, const size_t& count)
{
	std::sort(fixture.begin(), fixture.end());
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(SortVector, SortVectorFixture, sort_vector_args);



//
// dot product
//

static const std::vector<size_t> dot_product_args{
	1000,
	10000,
	100000,
	1000000,
	10000000
};

class DotProductFixture
{
public:
	struct Type
	{
		std::vector<double> v1;
		std::vector<double> v2;
	};

	DotProductFixture() {}

	Type& SetUp(const size_t& count)
	{
		if (fixture_.v1.size() != count)
		{
			fixture_.v1.resize(count, 1.0);
			fixture_.v2.resize(count, 0.0);
		}
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

static void DotProduct(DotProductFixture::Type& fixture, const size_t& count)
{
	auto& v1 = fixture.v1;
	auto& v2 = fixture.v2;
	double res = 0.0;
	for (size_t i = 0; i < count; ++i)
		res += v1[i] * v2[i];
	v1.back() = res;
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(DotProduct, DotProductFixture, dot_product_args);



//
// naive DGEMM
//

static const std::vector<size_t> dgemm_args{
	32,
	64,
	96,
	128,
	256,
	300,
	400,
	512,
	800,
};

class DGEMMFixture
{
public:
	struct Type
	{
		std::vector<double> a;
		std::vector<double> b;
		std::vector<double> c;
	};

	DGEMMFixture() {}

	Type& SetUp(const size_t& count)
	{
		if (fixture_.a.size() != count * count)
		{
			fixture_.a.resize(count * count, 0.0);
			fixture_.b.resize(count * count, 0.0);
			fixture_.c.resize(count * count, 0.0);
		}
		return fixture_;
	}

	void TearDown() {}

private:
	Type fixture_;
};

static void NaiveDGEMM(DGEMMFixture::Type& fixture, const size_t& count)
{
	auto& a = fixture.a;
	auto& b = fixture.b;
	auto& c = fixture.c;
	for (size_t i = 0; i < count; ++i)
	{
		const size_t fcol = i * count;
		for (size_t j = 0; j < count; ++j)
		{
			double c_val = 0.0;
			for (size_t k = 0; k < count; ++k)
				c_val += a[fcol + k] * b[k * count + j];
			c[fcol + j] = c_val;
		}
	}
}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(NaiveDGEMM, DGEMMFixture, dgemm_args);
