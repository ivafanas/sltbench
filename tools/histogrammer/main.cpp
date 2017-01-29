#include "HistogramBuilder.h"

#include <set>


static void InsertToSetSorted()
{
	std::set<size_t> s;
	for (size_t i = 0; i < 1000; ++i)
		s.insert(i);
}

int main(int argc, char **argv)
{
	sltbench::analytics::HistogramBuilder histogram_builder;

	histogram_builder.BuildFor(&InsertToSetSorted, "inserttosetsorted");

	return 0;
}
