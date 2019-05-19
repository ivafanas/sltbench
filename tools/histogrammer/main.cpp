#include "HistogramBuilder.h"

#include <exception>
#include <iostream>
#include <set>


static void InsertToSetSorted()
{
	std::set<size_t> s;
	for (size_t i = 0; i < 1000; ++i)
		s.insert(i);
}

int main(int argc, char **argv)
{
	try
	{
		sltbench::analytics::HistogramBuilder histogram_builder;
		histogram_builder.BuildFor(&InsertToSetSorted, "inserttosetsorted");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
