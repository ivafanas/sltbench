#pragma once

#include <string>


namespace sltbench {

enum class Verdict
{
	OK,
	CRASHED,
	IMPRECISE,
};

std::string ToString(Verdict v);

} // namespace sltbench
