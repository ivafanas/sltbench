#pragma once

#include <map>


namespace sltbench {

class MAResultsContainer
{
public:
	MAResultsContainer();
	~MAResultsContainer() noexcept;

public:
	//! Add timing result to container
	void Add(uint64_t value);

	//! Return the best timing result or zero if no results
	uint64_t GetBest() const noexcept;

	//! Return value in spot or zero is such value does not exist
	uint64_t GetMinSpotValue(
		size_t spot_size,
		uint8_t precision_percents) const noexcept;

private:
	// nanoseconds to count of results with the quite same value
	std::map<uint64_t, size_t> ns_to_count_;

	// total count of results (cached value of ns_to_count_ sum)
	size_t total_count_ = 0;
};

} // namespace sltbench
