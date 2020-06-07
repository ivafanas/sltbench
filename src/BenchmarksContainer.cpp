#include <sltbench/impl/BenchmarksContainer.h>
#include "BenchmarksContainerPrivate.h"

#include <sltbench/impl/IBenchmark.h>

#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <utility>


namespace sltbench {
namespace {

class Container
{
public:
	static Container& Instance()
	{
		static Container container;
		return container;
	}

	void Add(std::unique_ptr<IBenchmark>&& benchmark)
	{
		const std::string name{ benchmark->name };
		const auto it = name_to_bm_.find(name);
		if (it == name_to_bm_.end())
		{
			name_to_bm_[name] = std::move(benchmark);
		}
		else
		{
			std::fprintf(stderr,
			             "WARNING: Benchmark with name %s is already registered.\n"
			             "Namesake benchmarks are ignored!!\n",
			             benchmark->name);
		}
	}

	std::vector<IBenchmark*> Get() const
	{
		std::vector<IBenchmark*> rv;
		rv.reserve(name_to_bm_.size());
		for (const auto& name_and_bm : name_to_bm_)
			rv.push_back(name_and_bm.second.get());
		return rv;
	}

private:
	std::map<std::string, std::unique_ptr<IBenchmark>> name_to_bm_;
};

}  // namespace


void RegisterBenchmark(IBenchmark* benchmark)
{
    Container::Instance().Add(std::unique_ptr<IBenchmark>(benchmark));
}

std::vector<IBenchmark*> GetRegisteredBenchmarks()
{
	return Container::Instance().Get();
}

}  // namespace sltbench

