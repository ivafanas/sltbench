#pragma once

#include <string>


namespace sltbench {
namespace analytics {

class HistogramBuilder
{
public:
	HistogramBuilder() {}

public:
	/*!
		Run function {fun} several times,
		Write results to {filename}.dat,
		Write gnuplot histogram builder script to {filename}.plt
	 */
	void BuildFor(void (*fun)(), const std::string& filename);
};

} // analytics
} // namespace sltbench
