#pragma once

#include <sltbench/impl/IRunner.h>

#include <vector>


namespace sltbench {

std::vector<IRunnerPtr> GetRunners();

} // namespace sltbench
