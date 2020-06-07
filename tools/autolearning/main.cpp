#include <sltbench/Bench.h>
#include <sltbench/impl/Env.h>

#include "src/Config.h"
#include "src/MeasureAlgo.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <numeric>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>


using namespace sltbench;


namespace {

using TimingMap = std::map<std::string, std::chrono::nanoseconds>;

// do not report anything to stdout, collect resutls
class LearningReporter
	: public reporter::IReporter
{
public:
	void Clear()
	{
		test_2_res_.clear();
	}

	TimingMap GetResults()
	{
		return test_2_res_;
	}

public: // IReporter
	void ReportBenchmarkStarted() override {}
	void ReportBenchmarkFinished() override {}
	void Report(
		const std::string& name,
		const std::string& param,
		Verdict verdict,
		std::chrono::nanoseconds result) override
	{
		test_2_res_[name + "_" + param] = result;
	}
	void ReportWarning(RunWarning) override {}

private:
	TimingMap test_2_res_;
};
SLTBENCH_CONFIG().SetReporter(std::unique_ptr<LearningReporter>(new LearningReporter()));


//! convert time to human-readable (and not precise) string
std::string TimeToHRStr(std::chrono::nanoseconds ns)
{
	const auto count = ns.count();
	if (count < 1000)
		return std::to_string(count) + " (ns)";

	if (count < 1000000)
		return std::to_string(count / 1000) + " (mcs)";

	if (count < 1000000000)
		return std::to_string(count / 1000000) + " (ms)";

	return std::to_string(count / 1000000000) + " (s)";
}


//! make benchmark run and collect results
TimingMap CollectResultsForRun(LearningReporter& reporter)
{
	reporter.Clear();
	Run();
	return reporter.GetResults();
}

//! check whether configuration is stable
struct LearningCheckResult
{
	bool is_stable = false;
};
LearningCheckResult MakeLearningCheck(MeasureAlgo::Conf conf, LearningReporter& reporter)
{
	const auto precisoion_percents = conf.precision_percents;
	Config::Instance().SetMeasureAlgoConf(std::move(conf));

	TimingMap timing_mins;
	TimingMap timing_maxs;

	std::cout << "testing configuration: " << std::endl;

	LearningCheckResult rv;
	rv.is_stable = true;
	// run 4 times to ensure results stability
	for (size_t i = 0; i < 4; ++i)
	{
		std::cout << "  run test" << std::endl;
		const auto timings = CollectResultsForRun(reporter);

		for (const auto& func_and_res : timings)
		{
			const auto& func = func_and_res.first;
			const auto& res = func_and_res.second;
			if (timing_mins.count(func) == 0)
			{
				timing_mins[func] = res;
				timing_maxs[func] = res;
			}
			else
			{
				auto& min_ns = timing_mins.find(func)->second;
				auto& max_ns = timing_maxs.find(func)->second;

				min_ns = std::min(min_ns, res);
				max_ns = std::max(max_ns, res);

				if (max_ns.count() > 0)
				{
					if (100 * (max_ns.count() - min_ns.count()) > precisoion_percents * max_ns.count())
					{
						std::cout << "  unstable configuration" << std::endl;
						std::cout << "  unstable test: " << func << std::endl;
						std::cout << "    min: " << TimeToHRStr(min_ns) << std::endl;
						std::cout << "    max: " << TimeToHRStr(max_ns) << std::endl;
						rv.is_stable = false;
						return rv;
					}
				}
			}
		}
	}
	return rv;
}

size_t GenerateCandidateSpotSize(
	const size_t unstable_spot_size,
	const size_t stable_spot_size)
{
	if (unstable_spot_size >= stable_spot_size)
		return stable_spot_size;

	if (unstable_spot_size + 1 == stable_spot_size)
		return stable_spot_size;

	return (unstable_spot_size + stable_spot_size) / 2;
}

//! whether the new spot candidate can be generated
bool IsSpotSizeLearned(size_t unstable_stop_size, size_t stable_spot_size)
{
	return unstable_stop_size + 1 >= stable_spot_size;
}

//! which time segment is being learned
size_t IndexOfLearningDOTParam(
	const std::vector<MeasureAlgo::Conf::DOTParam>& candidate_params,
	const std::vector<MeasureAlgo::Conf::DOTParam>& stable_params)
{
	const size_t n = candidate_params.size();
	for (size_t i = 0; i < n; ++i)
	{
		const size_t ind = n - i - 1;
		if (candidate_params[ind].required_spot_size != stable_params[ind].required_spot_size)
			return ind;
	}
	return n;
}


//! generate the new candidate configuration to check whether it stable or not
std::unique_ptr<MeasureAlgo::Conf> GenerateCandidateConf(
	const MeasureAlgo::Conf& unstable_conf,
	const MeasureAlgo::Conf& stable_conf)
{
	MeasureAlgo::Conf rv;
	rv.max_execution_time = stable_conf.max_execution_time;
	rv.min_execution_time = stable_conf.min_execution_time;
	rv.precision_percents = stable_conf.precision_percents;

	bool found_diff = false;
	const size_t n = unstable_conf.dot_params.size();
	for (size_t i = 0; i < n; ++i)
	{
		const size_t ind = n - i - 1;
		const auto unstable_spot = unstable_conf.dot_params[ind].required_spot_size;
		const auto stable_spot = stable_conf.dot_params[ind].required_spot_size;
		if (!found_diff && !IsSpotSizeLearned(unstable_spot, stable_spot))
		{
			found_diff = true;
			rv.dot_params.insert(
				rv.dot_params.begin(),
				MeasureAlgo::Conf::DOTParam{
					stable_conf.dot_params[ind].min_time,
					GenerateCandidateSpotSize(unstable_spot, stable_spot) });
		}
		else
		{
			rv.dot_params.insert(rv.dot_params.begin(), stable_conf.dot_params[ind]);
		}
	}

	if (found_diff)
	{
		return std::unique_ptr<MeasureAlgo::Conf>(new MeasureAlgo::Conf(rv));
	}
	return nullptr;
}

std::ostream& operator << (std::ostream& os, const MeasureAlgo::Conf& conf)
{
	os << "{\n";
	os << "  precision_percents: " << static_cast<unsigned>(conf.precision_percents) << '\n';
	os << "  min_execution_time: " << TimeToHRStr(conf.min_execution_time) << '\n';
	os << "  max_execution_time: " << TimeToHRStr(conf.max_execution_time) << '\n';
	os << "  dot_params: {\n";
	for (const auto& dot_param : conf.dot_params)
	{
		os
			<< "    { "
			<< dot_param.required_spot_size
			<< ", "
			<< TimeToHRStr(dot_param.min_time)
			<< "}\n";
	}
	os << "  }\n";
	os << "}\n";
	return os;
}

void heatup_function()
{
	const size_t count = 1024 * 1024;
	std::vector<unsigned> vec;
	vec.reserve(count);
	for (size_t i = 0; i < count; ++i)
		vec.push_back(i % 1024);
	std::sort(vec.begin(), vec.end());
}

void heatup()
{
	const std::chrono::nanoseconds heatup_limit = std::chrono::seconds(7);
	std::chrono::nanoseconds heating_time(0);
	while (heating_time < heatup_limit)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		heatup_function();
		auto final_time = std::chrono::high_resolution_clock::now();
		heating_time += (final_time - start_time);
	}
}

template<typename Predicate>
void SetMeasureRequiredPredicate(Predicate&& p)
{
	// TODO: implement this without affecting end user
}

} // namespace


int main(int argc, char **argv)
{
	Env::Instance().SetArgs(argc, argv);

	auto& config = Config::Instance();
	auto& reporter = config.GetReporter();
	auto& custom_reporter = dynamic_cast<LearningReporter&>(reporter);

	// some kind of dark magic: heatup core and scheduler
	std::cout << "heating up..." << std::endl;
	heatup();

	// we did the custom heatup, so
	// heatup-per-run is not required
	Config::Instance().is_heatup_required = false;

	MeasureAlgo::Conf stable_conf;
	stable_conf.precision_percents = 5;
	stable_conf.max_execution_time = std::chrono::minutes(1);
	stable_conf.min_execution_time = std::chrono::milliseconds(250);
	stable_conf.dot_params = {
		{ std::chrono::seconds(3), 1 },
		{ std::chrono::milliseconds(250), 3 },
		{ std::chrono::milliseconds(100), 3 },
		{ std::chrono::milliseconds(50),  4 },
		{ std::chrono::milliseconds(20),  5 },
		{ std::chrono::milliseconds(10),  10 },
		{ std::chrono::milliseconds(1),   15 },
		{ std::chrono::microseconds(500), 15 },
		{ std::chrono::microseconds(250), 15 },
		{ std::chrono::microseconds(100), 18 },
		{ std::chrono::microseconds(50),  20 },
		{ std::chrono::nanoseconds::zero(), 20 }
	};

	MeasureAlgo::Conf unstable_conf;
	unstable_conf.precision_percents = 5;
	unstable_conf.max_execution_time = std::chrono::minutes(1);
	unstable_conf.min_execution_time = std::chrono::milliseconds(250);
	unstable_conf.dot_params = {
		{ std::chrono::seconds(3), 1 },
		{ std::chrono::milliseconds(250), 2 },
		{ std::chrono::milliseconds(100), 3 },
		{ std::chrono::milliseconds(50),  4 },
		{ std::chrono::milliseconds(20),  5 },
		{ std::chrono::milliseconds(10),  10 },
		{ std::chrono::milliseconds(1),   15 },
		{ std::chrono::microseconds(500), 15 },
		{ std::chrono::microseconds(250), 15 },
		{ std::chrono::microseconds(100), 18 },
		{ std::chrono::microseconds(50),  20 },
		{ std::chrono::nanoseconds::zero(), 20 }
	};

	// TODO: SetMeasureRequiredPred is not supported more
	//       make clear how to organize autolearning utility
	//       without affecting end user
	// check that stable_conf is really stable
	std::cout << "check stable conf is really stable:\n" << stable_conf;
	SetMeasureRequiredPredicate([](std::chrono::nanoseconds) { return true; });
	bool is_stable_conf_stable = MakeLearningCheck(stable_conf, custom_reporter).is_stable;
	if (is_stable_conf_stable)
	{
		std::cout << "ok, initial stable conf is stable\n";
		std::cout << "start learning\n";
		while (true)
		{
			auto cand_conf = GenerateCandidateConf(unstable_conf, stable_conf);
			if (!cand_conf)
				break;

			// filter functions that do not fit into diff between candidate and stable
			SetMeasureRequiredPredicate([&](std::chrono::nanoseconds ns) {
				const size_t dots_count = cand_conf->dot_params.size();
				for (size_t i = 0; i < dots_count; ++i)
				{
					const size_t n = dots_count - i - 1;

					// check that estimation time fits into n-th segment
					bool target_segment = false;
					if (n == 0)
					{
						target_segment = ns >= cand_conf->dot_params[n].min_time;
					}
					else
					{
						target_segment =
							cand_conf->dot_params[n].min_time <= ns &&
							cand_conf->dot_params[n-1].min_time > ns;
					}

					// check that n-th segment params differs between candidate and stable
					if (target_segment)
					{
						return
							cand_conf->dot_params[n].required_spot_size !=
							stable_conf.dot_params[n].required_spot_size;
					}
				}
				return false;
			});

			std::cout << std::endl;
			std::cout << "testing conf: " << *cand_conf;
			std::cout << std::endl;

			const auto check_res = MakeLearningCheck(*cand_conf, custom_reporter);

			const auto learn_ind = IndexOfLearningDOTParam(
				cand_conf->dot_params,
				stable_conf.dot_params);
			if (check_res.is_stable)
			{
				std::cout << "  ok: configuration is stable";
				stable_conf.dot_params[learn_ind].required_spot_size =
					cand_conf->dot_params[learn_ind].required_spot_size;
			}
			else
			{
				std::cout << "  DENY: configuration is unstable";
				unstable_conf.dot_params[learn_ind].required_spot_size =
					cand_conf->dot_params[learn_ind].required_spot_size;
			}
			std::cout << std::endl;
		}

		std::cout << "\n\n\n";
		std::cout << "last stable conf is: " << stable_conf << std::endl;
	}
	else
	{
		std::cout << "FAILED: initial conf proposed as stable is NOT stable. Learning aborted\n";
		return 1;
	}
	return 0;
}
