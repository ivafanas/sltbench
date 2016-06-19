#pragma once

#include <functional>
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
	void BuildFor(const std::function<void()>& fun, const std::string& filename);
};

} // analytics
} // namespace sltbench
