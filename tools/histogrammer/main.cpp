#include "HistogramBuilder.h"

#include <iostream>
#include <set>


static void InsertToSetSorted(const size_t& count)
{
	std::set< size_t > s;
	for (size_t i = 0; i < count; ++i)
		s.insert(i);
}

int main(int argc, char **argv)
{
	sltbench::analytics::HistogramBuilder histogram_builder;

	for (auto n : { 10000, 50000, 500000, 700000, 900000, 1000000, })
	{
		std::cout << "build for: " << "inserttosetsorted_" + std::to_string(n) << std::endl;
		histogram_builder.BuildFor([&]() { InsertToSetSorted(n); }, "inserttosetsorted_" + std::to_string(n));
	}

	return 0;
}
