#pragma once

#include <algorithm>
#include <initializer_list>

// test-specific algorithms
// mostly to emulate ranges and avoid dependency on gmock

namespace sltbench {

template<typename Sequence, typename Value>
bool test_sequence_is(const Sequence& s, std::initializer_list<Value> values)
{
	// std::equal(first1, last1, first2, last2) is since C++14 only:
	// https://en.cppreference.com/w/cpp/algorithm/equal
	if (s.size() != values.size())
		return false;
	return std::equal(s.begin(), s.end(), values.begin());
}

}  // namespace sltbench

