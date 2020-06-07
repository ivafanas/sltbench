#include <sltbench/impl/IBenchmark.h>


namespace sltbench {

IBenchmark::IBenchmark(const char* const i_name,
                       const bool i_supports_multicall) noexcept
    : name(i_name)
	, supports_multicall(i_supports_multicall)
{}

IBenchmark::~IBenchmark() noexcept = default;

}  // namespace sltbench

