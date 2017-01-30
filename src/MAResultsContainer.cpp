#include "MAResultsContainer.h"

#include <iterator>


namespace sltbench {

MAResultsContainer::MAResultsContainer() = default;

MAResultsContainer::~MAResultsContainer() noexcept = default;

void MAResultsContainer::Add(const uint64_t time_ns)
{
	++total_count_;

	const uint64_t lower_value = ( 995 * time_ns) / 1000;
	const uint64_t upper_value = (1005 * time_ns) / 1000;
	auto it = ns_to_count_.lower_bound(lower_value);
	if (it == ns_to_count_.end() || // all values are very small
		it->first > upper_value) // next greater value is too big
	{
		ns_to_count_[time_ns] = 1;
	}
	else
	{
		// value fits to existing bucket,
		// put it to the bucket
		++(it->second);
	}
}

uint64_t MAResultsContainer::GetMinSpotValue(
	const size_t spot_size,
	const uint8_t precision_percents) const noexcept
{
	if (total_count_ < spot_size)
		return 0;

	const auto it_end = ns_to_count_.end();
	for (auto it = ns_to_count_.begin(); it != it_end; ++it)
	{
		const uint64_t ref_result = it->first;
		size_t fits_count = it->second;
		auto jt = std::next(it);
		while (true)
		{
			if (fits_count >= spot_size)
				break;

			if (jt == it_end)
				break;

			const auto diff = jt->first - ref_result;
			if (diff > (ref_result * precision_percents) / 100)
				break;

			fits_count += jt->second;
			++jt;
		}
		if (fits_count >= spot_size)
			return ref_result;
	}

	return 0;
}

uint64_t MAResultsContainer::GetBest() const noexcept
{
	if (!ns_to_count_.empty())
		return ns_to_count_.begin()->first;
	return 0;
}

} // namespace sltbench
