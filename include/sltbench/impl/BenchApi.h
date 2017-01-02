#pragma once

#include "BenchmarksContainer.h"
#include "BenchmarkWithArgGenerator.h"
#include "BenchmarkWithFixture.h"
#include "BenchmarkWithFixtureAndArgGenerator.h"
#include "BenchmarkWithFixtureAndLazyArgGenerator.h"
#include "BenchmarkWithLazyArgGenerator.h"
#include "Descriptor.h"
#include "IConfig.h"
#include "IRunner.h"
#include "StaticAssertsUtil.h"
#include "StopGenerationException.h"

#include <functional>
#include <type_traits>
#include <vector>


namespace sltbench {

using SLTFun = std::function<void()>;


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
    Register function with lazy args generator for benchmarking

    \param name - function name
    \param func - function for benchmarking

    \param GeneratorT - type of input values generator
*/
template<typename GeneratorT>
Descriptor *RegisterBenchmarkWithLazyArgsGenerator(
    const char *name,
    std::function<void(const typename GeneratorT::ArgType)> func)
{
    using BM = BenchmarkWithLazyArgGenerator<GeneratorT>;
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


/*!
    Register function with fixture and lazy args generator for benchmarking

    \param name - function name
    \param func - function for benchmarking

    \param FixtureT   - fixture class
    \param GeneratorT - input values generator class
*/
template<typename FixtureT, typename GeneratorT>
Descriptor *RegisterBenchmarkWithFixtureAndLazyArgsGenerator(
    const char *name,
    std::function<void(typename FixtureT::Type&, const typename GeneratorT::ArgType&)> func)
{
    using BM = BenchmarkWithFixtureAndLazyArgGenerator<FixtureT, GeneratorT>;
    BenchmarksContainer<BM>::Instance().Add(BM(name, func));

    Runner<BM>::Register();

    static Descriptor dscr;
    return &dscr;
}

} // namespace sltbench


//
// define internal helpers
//

#define SLT_CONCATENATE_2_IMPL(s1, s2) s1##s2
#define SLT_CONCATENATE_2(s1, s2) SLT_CONCATENATE_2_IMPL(s1, s2)

#define SLT_DECLARE_UNIQUE_CONFIG() ::sltbench::IConfig& SLT_CONCATENATE_2(conf_,__LINE__)
#define SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION(func) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc_##func,__LINE__)
#define SLT_DECLARE_UNIQUE_DESCRIPTOR_FUNCTION_WITH_FIXTURE(func, fixture) ::sltbench::Descriptor* SLT_CONCATENATE_2(desc___fix_##fixture##func,__LINE__)

#define SLT_REGISTER_FUNCTION(func) ::sltbench::RegisterBenchmark(#func, func)
#define SLT_REGISTER_FUNCTION_WITH_FIXTURE(func, fixture) ::sltbench::RegisterBenchmarkWithFixture<fixture>(#func"_"#fixture, func);
#define SLT_REGISTER_FUNCTION_WITH_ARGS(func, args_vec) ::sltbench::RegisterBenchmarkWithArgs(#func, std::function<void( const decltype(args_vec)::value_type &)>(func), args_vec)
#define SLT_REGISTER_FUNCTION_WITH_ARGS_GENERATOR(func, generator) ::sltbench::RegisterBenchmarkWithArgsGenerator<generator>(#func, func)
#define SLT_REGISTER_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func, generator) ::sltbench::RegisterBenchmarkWithLazyArgsGenerator<generator>(#func, func)
#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS(func, fixture, args_vec) ::sltbench::RegisterBenchmarkWithFixtureAndArgs<fixture>(#func"_"#fixture, std::function<void( fixture::Type&, const decltype(args_vec)::value_type &)>(func), args_vec);
#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func, fixture, generator) ::sltbench::RegisterBenchmarkWithFixtureAndArgsGenerator<fixture, generator>(#func"_"#fixture, func);
#define SLT_REGISTER_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func, fixture, generator) ::sltbench::RegisterBenchmarkWithFixtureAndLazyArgsGenerator<fixture, generator>(#func"_"#fixture, func);

#define SLT_FUN_ARITY(fun) sltbench::function_traits<decltype(&fun)>::arity
#define SLT_FUN_RETT(fun) sltbench::function_traits<decltype(&fun)>::return_t
#define SLT_FUN_ARGT(fun, arg_index) sltbench::function_traits<decltype(&fun)>::argument<(arg_index)>::type

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
