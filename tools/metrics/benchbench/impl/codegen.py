

_SLTBENCH_MATH_TMPL = '''
#include <sltbench/Bench.h>
#include <cstdlib>

static const std::vector<size_t> args{{
    {args}
}};

static void {func_name}(const size_t& arg)
{{
    const size_t n = arg;
    volatile size_t rv = 0;
    for (size_t i = 0; i < n; ++i)
        rv += std::rand() % 2;
}}
SLTBENCH_FUNCTION_WITH_ARGS({func_name}, args);
'''


def _gen_sltbench_test_math(name, begin, end):
    return _SLTBENCH_MATH_TMPL.format(
        func_name=name,
        args=', '.join(map(str, range(begin, end))))


_GOOGLEBENCH_MATH_TMPL = '''
#include <benchmark/benchmark.h>

static void CustomArgs(benchmark::internal::Benchmark* b)
{{
    for (int i = {begin}; i < {end}; ++i)
        b->Args({{i}});
}}

static void {func_name}(benchmark::State& state)
{{
    const auto size = state.range(0);
    while (state.KeepRunning())
    {{
        volatile size_t rv = 0;
        for (size_t i = 0; i < size; ++i)
            rv += std::rand() % 2;
    }}
}}
BENCHMARK({func_name})->Apply(CustomArgs);
'''


def _gen_googlebench_test_math(name, begin, end):
    return _GOOGLEBENCH_MATH_TMPL.format(func_name=name, begin=begin, end=end)


_SLTBENCH_MALLOC_TMPL = '''
#include <sltbench/Bench.h>
#include <set>

static const std::vector<size_t> args{{
    {args}
}};

static void {func_name}(const size_t& arg)
{{
    const size_t n = arg;
    std::set<size_t> s;
    for (size_t i = 0; i < n; ++i)
        s.insert(i);
}}
SLTBENCH_FUNCTION_WITH_ARGS({func_name}, args);
'''


def _gen_sltbench_test_malloc(name, begin, end):
    return _SLTBENCH_MALLOC_TMPL.format(
        func_name=name,
        args=', '.join(map(str, range(begin, end))))


_GOOGLEBENCH_MALLOC_TMPL = '''
#include <benchmark/benchmark.h>
#include <set>

static void CustomArgs(benchmark::internal::Benchmark* b)
{{
    for (int i = {begin}; i < {end}; ++i)
        b->Args({{i}});
}}

static void {func_name}(benchmark::State& state)
{{
    const auto size = state.range(0);
    while (state.KeepRunning())
    {{
        std::set<size_t> s;
        for (size_t i = 0; i < size; ++i)
            s.insert(i);
    }}
}}
BENCHMARK({func_name})->Apply(CustomArgs);
'''


def _gen_googlebench_test_malloc(name, begin, end):
    return _GOOGLEBENCH_MALLOC_TMPL.format(func_name=name, begin=begin, end=end)


SLTBENCH_TEST_MATH_MICRO  = _gen_sltbench_test_math('func_math_micro',  1, 51)
SLTBENCH_TEST_MATH_SMALL  = _gen_sltbench_test_math('func_math_small',  501, 551)
SLTBENCH_TEST_MATH_MEDIUM = _gen_sltbench_test_math('func_math_medium', 5001, 5051)
SLTBENCH_TEST_MATH_LARGE  = _gen_sltbench_test_math('func_math_large',  50001, 50051)

GOOGLEBENCH_TEST_MATH_MICRO  = _gen_googlebench_test_math('func_math_micro' , 1, 51)
GOOGLEBENCH_TEST_MATH_SMALL  = _gen_googlebench_test_math('func_math_small' , 501, 551)
GOOGLEBENCH_TEST_MATH_MEDIUM = _gen_googlebench_test_math('func_math_medium', 5001, 5051)
GOOGLEBENCH_TEST_MATH_LARGE  = _gen_googlebench_test_math('func_math_large' , 50001, 50051)

SLTBENCH_TEST_MALLOC_MICRO  = _gen_sltbench_test_malloc('func_malloc_micro' , 1, 51)
SLTBENCH_TEST_MALLOC_SMALL  = _gen_sltbench_test_malloc('func_malloc_small' , 501, 551)
SLTBENCH_TEST_MALLOC_MEDIUM = _gen_sltbench_test_malloc('func_malloc_medium', 5001, 5051)
SLTBENCH_TEST_MALLOC_LARGE  = _gen_sltbench_test_malloc('func_malloc_large' , 50001, 50051)

GOOGLEBENCH_TEST_MALLOC_MICRO  = _gen_googlebench_test_malloc('func_malloc_micro' , 1, 51)
GOOGLEBENCH_TEST_MALLOC_SMALL  = _gen_googlebench_test_malloc('func_malloc_small' , 501, 551)
GOOGLEBENCH_TEST_MALLOC_MEDIUM = _gen_googlebench_test_malloc('func_malloc_medium', 5001, 5051)
GOOGLEBENCH_TEST_MALLOC_LARGE  = _gen_googlebench_test_malloc('func_malloc_large' , 50001, 50051)
