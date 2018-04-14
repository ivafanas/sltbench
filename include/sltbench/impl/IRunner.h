#pragma once

#include "BenchmarksContainer.h"
#include "IConfig.h"
#include "IFilter.h"
#include "IReporter.h"
#include "SingleMeasureAlgo.h"
#include "Verdict.h"

#include <chrono>
#include <memory>


namespace sltbench {

//! benchmarks runner interface
struct IRunner
{
	virtual ~IRunner() = default;

	virtual bool Run(reporter::IReporter& reporter, IFilter& filter) = 0;
};
using IRunnerPtr = std::shared_ptr<IRunner>;


//! regsiter benchmarks runner
void RegisterRunner(const char *tag, const IRunnerPtr& runner);


template<typename BenchmarkT>
class Runner : public IRunner
{
public:
	static void Register()
	{
		RegisterRunner(
#ifdef __GNUC__
			__PRETTY_FUNCTION__
#else
			__FUNCTION__
#endif
			, Instance());
	}

	static const IRunnerPtr& Instance()
	{
		static IRunnerPtr runner = std::make_shared<Runner<BenchmarkT>>();
		return runner;
	}

private:
	//! run benchmark for input value and return result
	struct RunResult
	{
		std::chrono::nanoseconds time_ns;

		Verdict verdict = Verdict::OK;
	};
	RunResult Run(BenchmarkT& bm)
	{
		namespace sma = single_measure_algo;

		RunResult rv;

		// user-defined functions might throw exceptions
		// we should fail gracefully
		try
		{
			const bool enable_multicall = bm.SupportsMulticall();
			const auto measure_func = [&bm](size_t calls_count) -> std::chrono::nanoseconds {
				return bm.Measure(calls_count);
			};

			// estimate execution time			
			const auto estimation = sma::Estimate(measure_func, enable_multicall);

			if (estimation.verdict == sma::EstimationResult::Verdict::CANNOT_BE_PRECISE)
				rv.verdict = Verdict::IMPRECISE;

			// opt for autolearning: check whether we need to benchmark function
			// in production it is always true
			bool need_measure = GetConfig().GetPrivate().IsMeasureRequired(estimation.result);
			if (!need_measure)
			{
				rv.time_ns = std::chrono::nanoseconds(1);
			}
			else
			{
				rv.time_ns = sma::Measure(measure_func, estimation);
			}
		}
		catch (const std::exception&)
		{
			rv.time_ns = std::chrono::nanoseconds(0);
			rv.verdict = Verdict::CRASHED;
		}
		return rv;
	}

	//! run benchmark for all input values and report result
	//! @return true if all runs successfully completed.
	bool Run(BenchmarkT& bm, reporter::IReporter& reporter)
	{
		bm.Prepare();

		bool ok = true;
		while (bm.HasArgsToProcess())
		{
			const auto res = Run(bm);

			reporter.Report(
				bm.GetName(),
				bm.CurrentArgAsString(),
				res.verdict,
				res.time_ns);

			if (res.verdict == Verdict::CRASHED)
				ok = false;

			bm.OnArgProcessed();
		}

		bm.Finalize();

		return ok;
	}

public:

	//! run all registered benchmarks of type BenchmarkT
	//! @return true is none of runs crashed
	virtual bool Run(reporter::IReporter& reporter, IFilter& filter) override
	{
		auto& bms = BenchmarksContainer<BenchmarkT>::Instance().GetAll();

		bool was_crash = false;
		for (auto& name_and_bm: bms)
		{
			const auto& name = name_and_bm.first;
			if (filter.ShouldRunBenchmark(name))
			{
				auto& bm = *(name_and_bm.second);
				bool res = Run(bm, reporter);
				if (!res)
					was_crash = true;
			}
		}
		return !was_crash;
	}
};

} // namespace sltbench
