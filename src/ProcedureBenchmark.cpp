#include "ProcedureBenchmark.h"


using namespace std::chrono;


namespace sltbench {

ProcedureBenchmark::ProcedureBenchmark(const char *name, SLTFun function)
	: name_(name)
	, function_(std::move(function))
{
}

const std::string& ProcedureBenchmark::GetName() const
{
	return name_;
}

nanoseconds ProcedureBenchmark::Measure()
{
	const auto start_tp = high_resolution_clock::now();
	function_();
	const auto final_tp = high_resolution_clock::now();
	return
		final_tp > start_tp
		? duration_cast<nanoseconds>(final_tp - start_tp)
		: nanoseconds(0);;
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
