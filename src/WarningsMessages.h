#pragma once

#include <sltbench/impl/RunWarnings.h>

namespace sltbench {

//! Returns pointer to statically allocated message string.
const char * MessageForWarning(RunWarning warning);

} // namespace sltbench
