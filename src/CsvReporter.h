#pragma once

#include <sltbench/impl/IReporter.h>


namespace sltbench {
namespace reporter {

class CsvReporter
	: public IReporter
{
public:
	CsvReporter();

public: // IReporter
	virtual void Report(
		const std::string& name,
		const std::string& params,
		bool ok,
		std::chrono::nanoseconds timing_result) override;
};

} // namespace reporter
} // namespace sltbench
