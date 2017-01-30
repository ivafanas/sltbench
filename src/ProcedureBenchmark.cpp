#include "ProcedureBenchmark.h"


using namespace std::chrono;


namespace sltbench {

ProcedureBenchmark::ProcedureBenchmark(const char *name, SLTFun function)
	: name_(name)
	, function_(function)
{
}

const std::string& ProcedureBenchmark::GetName() const
{
	return name_;
}

nanoseconds ProcedureBenchmark::Measure(size_t calls_count)
{
	const auto start_tp = high_resolution_clock::now();
	for (size_t i = 0; i < calls_count; ++i)
		function_();
	const auto final_tp = high_resolution_clock::now();
	return
		final_tp > start_tp
		? duration_cast<nanoseconds>(final_tp - start_tp)
		: nanoseconds(0);;
}

bool ProcedureBenchmark::SupportsMulticall() const
{
	return true;
}

void ProcedureBenchmark::Prepare()
{
	measured_ = false;
}

void ProcedureBenchmark::Finalize()
{
}

bool ProcedureBenchmark::HasArgsToProcess()
{
	return !measured_;
}

void ProcedureBenchmark::OnArgProcessed()
{
	measured_ = true;
}

std::string ProcedureBenchmark::CurrentArgAsString()
{
	return{};
}

} // namespace sltbenh
