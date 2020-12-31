#pragma once

#include <algorithm>

// extension for STL algorithms because ranges are not allowed in C++11

namespace sltbench {

template<typename Container, typename Value>
bool contains(const Container& c, const Value& v) {
	return std::find(c.begin(), c.end(), v) != c.end();
}

}  // namespace

