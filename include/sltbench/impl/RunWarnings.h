#pragma once

namespace sltbench {

enum class RunWarning
{
	// sltbench is compiled in DEBUG mode,
	// which is not the best way to measure performance.
	DEBUG_BUILD
};

} // namespace sltbench
