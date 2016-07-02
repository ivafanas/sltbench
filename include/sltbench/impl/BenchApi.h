#pragma once

#include "BenchmarksContainer.h"
#include "BenchmarkWithArgGenerator.h"
#include "BenchmarkWithFixture.h"
#include "BenchmarkWithFixtureAndArgGenerator.h"
#include "Descriptor.h"
#include "IConfig.h"
#include "IRunner.h"
#include "StaticAssertsUtil.h"

#include <functional>
#include <type_traits>
#include <vector>


namespace sltbench {

using SLTFun = std::function<void()>;


//! initialize benchmark from command line options
void Init(int argc, char **argv);


//! run benchmarking tool
int Run(bool heatup = true);


/*!
	Register function for benchmarking

	\param name - function name
	\param func - function for benchmarking
*/
Descriptor *RegisterBenchmark(const char *name, SLTFun func);


/*!
	Register function with fixture for benchmarking

	\param name - benchmark name
	\param func - function for benchmarking

	\param FixtureT - fixture class
*/
template<typename FixtureT>
Descriptor *RegisterBenchmarkWithFixture(
	const char *name,
	std::function<void(typename FixtureT::Type&)> func)
{
	using BM = BenchmarkWithFixture<FixtureT>;
	BenchmarksContainer<BM>::Instance().Add(BM(name, func));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


//! helper stub - generator which generates nothing
template<typename Type>
class EmptyArgsGenerator
{
public:
	typedef Type ArgType;

	EmptyArgsGenerator() = default;

	std::vector<ArgType> Generate(int , char **) { return{}; }
};


/*!
	Register function with args for benchmarking

	\param name - function name
	\param func - function for benchmarking
	\param args - vector of input values

	\param ArgumentT - type of input value
*/
template<typename ArgumentT>
Descriptor *RegisterBenchmarkWithArgs(
	const char *name,
	std::function<void(const ArgumentT &)> func,
	std::vector<ArgumentT> args)
{
	using BM = BenchmarkWithArgGenerator<EmptyArgsGenerator<ArgumentT>>;
	BenchmarksContainer<BM>::Instance().Add(BM(name, func, std::move(args)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with args generator for benchmarking

	\param name - function name
	\param func - function for benchmarking

	\param GeneratorT - type of input values generator
*/
template<typename GeneratorT>
Descriptor *RegisterBenchmarkWithArgsGenerator(
	const char *name,
	std::function<void(const typename GeneratorT::ArgType)> func)
{
	using BM = BenchmarkWithArgGenerator<GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(BM(name, func));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture and args for benchmarking

	\param name     - function name
	\param func     - function for benchmarking
	\param args_vec - vector of input values

	\param FixtureT  - fixture class
	\param ArgumentT - type of input value
*/
template<typename FixtureT, typename ArgumentT>
Descriptor *RegisterBenchmarkWithFixtureAndArgs(
	const char *name,
	std::function<void(typename FixtureT::Type&, const ArgumentT&)> func,
	std::vector<ArgumentT> args_vec)
{
	using BM = BenchmarkWithFixtureAndArgGenerator<FixtureT, EmptyArgsGenerator<ArgumentT>>;
	BenchmarksContainer<BM>::Instance().Add(BM(name, func, std::move(args_vec)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture and args generator for benchmarking

	\param name - function name
	\param func - function for benchmarking

	\param FixtureT   - fixture class
	\param GeneratorT - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmarkWithFixtureAndArgsGenerator(
	const char *name,
	std::function<void(typename FixtureT::Type&, const typename GeneratorT::ArgType&)> func)
{
	using BM = BenchmarkWithFixtureAndArgGenerator<FixtureT, GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(BM(name, func));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}

} // namespace sltbench


//
// define internal helpers
//

#define SLTBENCH_PRIVATE_CONCATENATE_2_IMPL(s1, s2) s1##s2
#define SLTBENCH_PRIVATE_CONCATENATE_2(s1, s2) SLTBENCH_PRIVATE_CONCATENATE_2_IMPL(s1, s2)

#define SLTBENCH_PRIVATE_DECLARE_UNIQUE_CONFIG() ::sltbench::IConfig& SLTBENCH_PRIVATE_CONCATENATE_2(conf_,__LINE__)
#define SLTBENCH_PRIVATE_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) ::sltbench::Descriptor* SLTBENCH_PRIVATE_CONCATENATE_2(desc_##func,__LINE__)
#define SLTBENCH_PRIVATE_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) ::sltbench::Descriptor* SLTBENCH_PRIVATE_CONCATENATE_2(desc___fix_##fixture##func,__LINE__)

#define SLTBENCH_PRIVATE_REGISTER_FUNCTION(func) ::sltbench::RegisterBenchmark(#func, func)
#define SLTBENCH_PRIVATE_REGISTER_FUNCTION_WITH_FIXTURE(func, fixture) ::sltbench::RegisterBenchmarkWithFixture<fixture>(#func"_"#fixture, func);
#define SLTBENCH_PRIVATE_REGISTER_FUNCTION_WITH_ARGS(func, args_vec) ::sltbench::RegisterBenchmarkWithArgs(#func, std::function<void( const decltype(args_vec)::value_type &)>(func), args_vec)
#define SLTBENCH_PRIVATE_REGISTER_FUNCTION_WITH_ARGS_GENERATOR(func, generator) ::sltbench::RegisterBenchmarkWithArgsGenerator<generator>(#func, func)
#define SLTBENCH_PRIVATE_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec) ::sltbench::RegisterBenchmarkWithFixtureAndArgs<fixture>(#func"_"#fixture, std::function<void( fixture::Type&, const decltype(args_vec)::value_type &)>(func), args_vec);
#define SLTBENCH_PRIVATE_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator) ::sltbench::RegisterBenchmarkWithFixtureAndArgsGenerator<fixture, generator>(#func"_"#fixture, func);

#define SLTBENCH_PRIVATE_STATIC_ASSERT_IS_FIXTURE(fixture) \
	static_assert(std::is_class<fixture>::value, "Fixture must be a class"); \
	static_assert(std::is_default_constructible<fixture>::value, "Fixture must be default constructible"); \
	static_assert(sltbench::SLT_HasInnerTypedef_Type<fixture>::value, "Fixture class must have inner typedef 'Type'"); \
	static_assert(std::is_member_function_pointer<decltype(&fixture::SetUp)>::value, "Fixture class must have 'Setup' method"); \
	static_assert(std::is_member_function_pointer<decltype(&fixture::TearDown)>::value, "Fixture class must have 'TearDown' method");

#define SLTBENCH_PRIVATE_STATIC_ASSERT_IS_GENERATOR(generator) \
	static_assert(std::is_class<generator>::value, "Arguments generator must be a class"); \
	static_assert(std::is_default_constructible<generator>::value, "Arguments generator must be default constructible"); \
	static_assert(sltbench::SLT_HasInnerTypedef_ArgType<generator>::value, "Arguments generator must have inner typedef 'ArgType'"); \
	static_assert(sltbench::SLT_HasInsertionOperator<generator::ArgType>::value, "Arguments must be insertable (operator <<)"); \
	static_assert(std::is_member_function_pointer<decltype(&generator::Generate)>::value, "Arguments generator must have 'Generate' method");

#define SLTBENCH_PRIVATE_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec) \
	static_assert(sltbench::SLT_HasInnerTypedef_value_type<decltype(args_vec)>::value, "Arguments must be a vector"); \
	static_assert(sltbench::SLT_HasInsertionOperator<decltype(args_vec)::value_type>::value, "Arguments must be insertable (operator <<)"); \
	static_assert(std::is_same<std::vector<std::remove_const<decltype(args_vec)::value_type>::type>, std::remove_const<decltype(args_vec)>::type>::value, "Arguments must be a vector");
