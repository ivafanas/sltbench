#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>


namespace sltbench {

template<typename BenchmarkT>
class BenchmarksContainer
{
public:
	// we need std::map to define execution order of benchmarks inside one container
	typedef std::map<std::string, std::unique_ptr<BenchmarkT>> BenchmarksMap;

public:
	static BenchmarksContainer<BenchmarkT> & Instance()
	{
		static BenchmarksContainer<BenchmarkT> inst;
		return inst;
	}

public:
	void Add(std::unique_ptr<BenchmarkT> bm)
	{
		const auto& name = bm->GetName();
		auto it = bms_.find(name);
		if (it == bms_.end())
		{
			bms_.insert(std::make_pair(name, std::move(bm)));
		}
		else
		{
			std::cerr << "WARNING: Benchmark with name " << name << " is already registered.\n";
			std::cerr << "Namesake benchmarks are ignored!!" << std::endl;
		}
	}

	BenchmarksMap& GetAll()
	{
		return bms_;
	}

private:
	BenchmarksMap bms_;
};

} // namespace sltbench
