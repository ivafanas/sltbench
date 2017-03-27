#pragma once

#include "Benchmark_AG.h"
#include "Benchmark_F.h"
#include "Benchmark_F_AG.h"
#include "Benchmark_F_LAG.h"
#include "Benchmark_FB.h"
#include "Benchmark_FB_AG.h"
#include "Benchmark_FB_LAG.h"
#include "Benchmark_LAG.h"
#include "BenchmarksContainer.h"
#include "DoNotOptimize.h"
#include "Descriptor.h"
#include "IConfig.h"
#include "IRunner.h"
#include "StaticAssertsUtil.h"
#include "StopGenerationException.h"

#include <type_traits>
#include <vector>


namespace sltbench {

typedef void(*SLTFun)();


//! calls Init and Run
int Main(int argc, char **argv);


//! initialize benchmark from command line options
void Init(int argc, char **argv);


//! run benchmarking tool
int Run();


/*!
	Register function for benchmarking

	\param name - function name
	\param func - function for benchmarking
*/
Descriptor *RegisterBenchmark(const char *name, SLTFun func);


/*!
	Register function with simplified fixture builder for benchmarking

	\param name - benchmark name
	\param func - function for benchmarking
	\param fixture_builder - function which returns fixture
*/
template<typename FixtureT>
Descriptor *RegisterBenchmark_FB(
	const char *name,
	void(*func)(FixtureT&),
	FixtureT(*fixture_builder)())
{
	using BM = Benchmark_FB<FixtureT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, fixture_builder)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture class for benchmarking

	\param name - benchmark name
	\param func - function for benchmarking

	\param FixtureT - fixture class
*/
template<typename FixtureT>
Descriptor *RegisterBenchmark_F(const char *name, void (*func)(typename FixtureT::Type&))
{
	using BM = Benchmark_F<FixtureT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

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
	~EmptyArgsGenerator() = default;

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
Descriptor *RegisterBenchmark_A(
	const char *name,
	void (*func)(const ArgumentT &),
	std::vector<ArgumentT> args)
{
	using BM = Benchmark_AG<EmptyArgsGenerator<ArgumentT>>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, std::move(args))));

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
Descriptor *RegisterBenchmark_AG(
	const char *name,
	void (*func)(const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_AG<GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
    Register function with lazy args generator for benchmarking

    \param name - function name
    \param func - function for benchmarking

    \param GeneratorT - type of input values generator
*/
template<typename GeneratorT>
Descriptor *RegisterBenchmark_LAG(
	const char *name,
	void (*func)(const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_LAG<GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

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
Descriptor *RegisterBenchmark_F_A(
	const char *name,
	void (*func)(typename FixtureT::Type&, const ArgumentT&),
	std::vector<ArgumentT> args_vec)
{
	using BM = Benchmark_F_AG<FixtureT, EmptyArgsGenerator<ArgumentT>>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, std::move(args_vec))));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture builder and args for benchmarking

	\param name            - function name
	\param func            - function for benchmarking
	\param fixture_builder - function returning fixture
	\param args_vec        - vector of input values
*/
template<typename FixtureT, typename ArgumentT>
Descriptor *RegisterBenchmark_FB_A(
	const char *name,
	void(*func)(FixtureT&, const ArgumentT&),
	FixtureT(*fixture_builder)(const ArgumentT&),
	std::vector<ArgumentT> args_vec)
{
	using BM = Benchmark_FB_AG<FixtureT, EmptyArgsGenerator<ArgumentT>>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, fixture_builder, std::move(args_vec))));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture and args generator for benchmarking

	\param name - function name
	\param func - function for benchmarking

	\param FixtureT   - type of the fixture
	\param GeneratorT - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmark_F_AG(
	const char *name,
	void (*func)(typename FixtureT::Type&, const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_F_AG<FixtureT, GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture builder and args generator for benchmarking

	\param name            - function name
	\param func            - function for benchmarking
	\param fixture_builder - function returning fixture

	\param GeneratorT      - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmark_FB_AG(
	const char *name,
	void(*func)(FixtureT&, const typename GeneratorT::ArgType&),
	FixtureT(*fixture_builder)(const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_FB_AG<FixtureT, GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, fixture_builder)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
    Register function with fixture and lazy args generator for benchmarking

    \param name - function name
    \param func - function for benchmarking

    \param FixtureT   - fixture class
    \param GeneratorT - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmark_F_LAG(
	const char *name,
	void (*func)(typename FixtureT::Type&, const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_F_LAG<FixtureT, GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}


/*!
	Register function with fixture builder and lazy args generator for benchmarking

	\param name            - function name
	\param func            - function for benchmarking
	\param fixture_builder - function returning fixture

	\param GeneratorT      - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmark_FB_LAG(
	const char *name,
	void(*func)(FixtureT&, const typename GeneratorT::ArgType&),
	FixtureT(*fixture_builder)(const typename GeneratorT::ArgType&))
{
	using BM = Benchmark_FB_LAG<FixtureT, GeneratorT>;
	BenchmarksContainer<BM>::Instance().Add(
		std::unique_ptr<BM>(new BM(name, func, fixture_builder)));

	Runner<BM>::Register();

	static Descriptor dscr;
	return &dscr;
}

} // namespace sltbench


//
// define internal helpers
//

#define SLT_IS_FUN(fun) std::is_function<decltype(fun)>::value
#define SLT_FUN_ARITY(fun) sltbench::function_traits<decltype(&fun)>::arity
#define SLT_FUN_RETT(fun) sltbench::function_traits<decltype(&fun)>::return_t
#define SLT_FUN_ARGT(fun, arg_index) sltbench::function_traits<decltype(&fun)>::argument<(arg_index)>::type

#define SLT_CONCATENATE_2_IMPL(s1, s2) s1##s2
#define SLT_CONCATENATE_2(s1, s2) SLT_CONCATENATE_2_IMPL(s1, s2)

#define SLT_DECLARE_UNIQUE_CONFIG() \
	::sltbench::IConfig& SLT_CONCATENATE_2(conf_,__LINE__)

#define SLT_DECLARE_DESCR_FUNCTION(func) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc_##func,__LINE__)
#define SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE(func, fixture) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc___fix_##fixture##func,__LINE__)
#define SLT_DECLARE_DESCR_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc___fix_builder_##fixture_builder##func,__LINE__)

#define SLT_REGISTER_FUNCTION(func) ::sltbench::RegisterBenchmark(#func, func)

#define SLT_REGISTER_FUNCTION_WITH_ARGS(func, args_vec) \
	::sltbench::RegisterBenchmark_A(#func, func, args_vec)

#define SLT_REGISTER_FUNCTION_WITH_ARGS_GENERATOR(func, generator) \
	::sltbench::RegisterBenchmark_AG<generator>(#func, func)

#define SLT_REGISTER_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, generator) \
	::sltbench::RegisterBenchmark_LAG<generator>(#func, func)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE(func, fixture) \
	::sltbench::RegisterBenchmark_F<fixture>(#func"_"#fixture, func)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec) \
	::sltbench::RegisterBenchmark_F_A<fixture>(#func"_"#fixture, func, args_vec)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator) \
	::sltbench::RegisterBenchmark_F_AG<fixture, generator>(#func"_"#fixture, func)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator) \
	::sltbench::RegisterBenchmark_F_LAG<fixture, generator>(#func"_"#fixture, func)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER(func, fixture_builder) \
	::sltbench::RegisterBenchmark_FB(#func"_"#fixture_builder, func, fixture_builder)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(func, fixture_builder, args_vec) \
	::sltbench::RegisterBenchmark_FB_A<SLT_FUN_RETT(fixture_builder), decltype(args_vec)::value_type>(#func"_"#fixture_builder, func, fixture_builder, args_vec)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(func, fixture_builder, generator) \
	::sltbench::RegisterBenchmark_FB_AG<SLT_FUN_RETT(fixture_builder), generator>(#func"_"#fixture_builder, func, fixture_builder)

#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(func, fixture_builder, generator) \
	::sltbench::RegisterBenchmark_FB_LAG<SLT_FUN_RETT(fixture_builder), generator>(#func"_"#fixture_builder, func, fixture_builder)

// whether fun is a member function of type 'return_t()'
#define SLT_IS_MEMFUN_ARG0(fun, return_t) \
	std::is_member_function_pointer<decltype(&fun)>::value && \
	std::is_same<SLT_FUN_RETT(fun), return_t>::value && \
	SLT_FUN_ARITY(fun) == 1

// whether fun is a member function of type 'return_t(arg_t)'
#define SLT_IS_MEMFUN_ARG1(fun, return_t, arg_t) \
	std::is_member_function_pointer<decltype(&fun)>::value && \
	SLT_FUN_ARITY(fun) == 2 && \
	std::is_same<SLT_FUN_RETT(fun), return_t>::value && \
	std::is_same<SLT_FUN_ARGT(fun, 1), arg_t>::value

// whether fun is a member function of type 'return_t(arg1_t, arg2_t)'
#define SLT_IS_MEMFUN_ARG2(fun, return_t, arg1_t, arg2_t) \
	std::is_member_function_pointer<decltype(&fun)>::value && \
	SLT_FUN_ARITY(fun) == 3 && \
	std::is_same<SLT_FUN_RETT(fun),  return_t>::value && \
	std::is_same<SLT_FUN_ARGT(fun, 1), arg1_t>::value && \
	std::is_same<SLT_FUN_ARGT(fun, 2), arg2_t>::value

#define SLT_STATIC_ASSERT_IS_FIXTURE(fixture) \
	static_assert(std::is_class<fixture>::value, "Fixture must be a class"); \
	static_assert(std::is_default_constructible<fixture>::value, "Fixture must be default constructible"); \
	static_assert(sltbench::has_inner_type_Type<fixture>::value, "Fixture class must have inner typedef 'Type'"); \
	static_assert(SLT_IS_MEMFUN_ARG0(fixture::SetUp, fixture::Type&), "Fixture class must have 'Type& Setup()' method"); \
	static_assert(SLT_IS_MEMFUN_ARG0(fixture::TearDown, void), "Fixture class must have 'void TearDown()' method");

#define SLT_STATIC_ASSERT_IS_ARG_FIXTURE(fixture, arg_type) \
	static_assert(std::is_class<fixture>::value, "Fixture must be a class"); \
	static_assert(std::is_default_constructible<fixture>::value, "Fixture must be default constructible"); \
	static_assert(sltbench::has_inner_type_Type<fixture>::value, "Fixture class must have inner typedef 'Type'"); \
	static_assert(SLT_IS_MEMFUN_ARG1(fixture::SetUp, fixture::Type&, const arg_type &), "Fixture class must have 'Type& Setup(const Arg&)' method"); \
	static_assert(SLT_IS_MEMFUN_ARG0(fixture::TearDown, void), "Fixture class must have 'void TearDown()' method");

#define SLT_STATIC_ASSERT_IS_GENERATOR(generator) \
	static_assert(std::is_class<generator>::value, "Arguments generator must be a class"); \
	static_assert(std::is_default_constructible<generator>::value, "Arguments generator must be default constructible"); \
	static_assert(sltbench::has_inner_type_ArgType<generator>::value, "Arguments generator must have inner typedef 'ArgType'"); \
	static_assert(SLT_IS_MEMFUN_ARG2(generator::Generate, std::vector<typename generator::ArgType>, int, char**), "Arguments generator must have 'std::vector<ArgType> Generate(int, char **)' method");

#define SLT_STATIC_ASSERT_IS_LAZY_GENERATOR(generator) \
	static_assert(std::is_class<generator>::value, "Lazy arguments generator must be a class"); \
	static_assert(sltbench::is_constructible_from_env<generator>::value, "Lazy arguments generator must have c-tor from (int argc, char **argv)"); \
	static_assert(sltbench::has_inner_type_ArgType<generator>::value, "Lazy arguments generator must have inner typedef 'ArgType'"); \
	static_assert(SLT_IS_MEMFUN_ARG0(generator::Generate, generator::ArgType), "Lazy arguments generator must have 'ArgType Generate()' method");

#define SLT_STATIC_ASSERT_IS_ARGS_VECTOR(args_vec) \
	static_assert(sltbench::has_inner_type_value_type<decltype(args_vec)>::value, "Arguments must be a vector"); \
	static_assert(std::is_same<std::vector<std::remove_const<decltype(args_vec)::value_type>::type>, std::remove_const<decltype(args_vec)>::type>::value, "Arguments must be a vector");

#define SLT_STATIC_ASSERT_IS_FIXTURE_BUILDER_FOR_ARGS(fixture_builder, arg_type) \
	static_assert(SLT_IS_FUN(fixture_builder), "Fixture builder must be a function"); \
	static_assert(SLT_FUN_ARITY(fixture_builder) == 1, "Fixture builder must take (const Arg&) argument"); \
	static_assert(std::is_same<SLT_FUN_ARGT(fixture_builder, 0), const arg_type &>::value, "Fixture builder must take (const Arg&) argument");
