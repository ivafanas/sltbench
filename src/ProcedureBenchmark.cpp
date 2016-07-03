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

nanoseconds ProcedureBenchmark::Measure(size_t)
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
}

void ProcedureBenchmark::Finalize()
{
}

size_t ProcedureBenchmark::GetArgsCount()
{
	return 1;
}

std::string ProcedureBenchmark::ConvertArgToString(size_t)
{
	return{};
}

} // namespace sltbenh
