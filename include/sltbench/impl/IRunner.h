#pragma once

#include "BenchmarksContainer.h"
#include "IConfig.h"
#include "IFilter.h"
#include "IReporter.h"

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
	typedef std::function<std::chrono::nanoseconds()> MeasureFun;

	//! estimate execution time
	std::chrono::nanoseconds Estimate(const MeasureFun& measure_func)
	{
		// estimaton should be at least 10 ms
		size_t ncalls = 0;
		std::chrono::nanoseconds accumulated_time = std::chrono::nanoseconds::zero();
		do
		{
			accumulated_time += measure_func();
			++ncalls;
		} while (accumulated_time < std::chrono::milliseconds(10));
		return accumulated_time / ncalls;
	}

	//! measure function time
	std::chrono::nanoseconds Measure(const MeasureFun& measure_func, std::chrono::nanoseconds estimation_time)
	{
		const auto measure_algo = GetConfig().GetPrivate().CreateMeasureAlgo();
		measure_algo->SetFirstTimingResult(estimation_time);
		while (measure_algo->NeedMoreTiming())
		{
			auto time_ns = measure_func();
			measure_algo->AddTimingResult(time_ns);
		}
		return measure_algo->GetResult();
	}

	//! run benchmark for input value and return result
	struct RunResult
	{
		bool ok = false;
		std::chrono::nanoseconds time_ns;
	};
	RunResult Run(BenchmarkT& bm, const size_t arg_index)
	{
		RunResult rv;

		// user-defined functions might throw exceptions
		// we should fail gracefully
		try
		{
			const auto measure_func = [&bm, arg_index]() -> std::chrono::nanoseconds {
				return bm.Measure(arg_index);
			};

			// estimate execution time
			const auto estimated_time = Estimate(measure_func);

			// opt for autolearning: check whether we need to benchmark function
			// in production it is always true
			const auto bm_id = bm.GetName() + "_" + bm.ConvertArgToString(arg_index);
			bool need_measure = GetConfig().GetPrivate().IsMeasureRequired(estimated_time);
			if (!need_measure)
			{
				rv.time_ns = std::chrono::nanoseconds(1);
			}
			else
			{
				rv.time_ns = Measure(measure_func, estimated_time);
			}

			rv.ok = true;
		}
		catch (const std::exception&)
		{
			rv.time_ns = std::chrono::nanoseconds(0);
			rv.ok = false;
		}
		return rv;
	}

	//! run benchmark for all input values and report result
	bool Run(BenchmarkT& bm, reporter::IReporter& reporter)
	{
		bm.Prepare();

		bool ok = true;
		const size_t args_count = bm.GetArgsCount();
		for (size_t i = 0; i < args_count; ++i)
		{
			const auto res = Run(bm, i);

			reporter.Report(
				bm.GetName(),
				bm.ConvertArgToString(i),
				res.ok,
				res.time_ns);

			if (!res.ok)
				ok = false;
		}

		bm.Finalize();

		return ok;
	}

public:
	//! run all registered benchmarks of type BenchmarkT
	virtual bool Run(reporter::IReporter& reporter, IFilter& filter) override
	{
		auto& bms = BenchmarksContainer<BenchmarkT>::Instance().GetAll();

		bool was_crash = false;
		for (auto& name_and_bm: bms)
		{
			const auto& name = name_and_bm.first;
			if (filter.ShouldRunBenchmark(name))
			{
				auto& bm = name_and_bm.second;
				bool res = Run(bm, reporter);
				if (!res)
					was_crash = true;
			}
		}
		return !was_crash;
	}
};

} // namespace sltbench
