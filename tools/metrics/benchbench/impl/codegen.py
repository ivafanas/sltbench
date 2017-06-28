

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


def __gen_nonius_test_math(name, begin, end):
    header = '''
    #include <nonius/nonius.h++>

    static void {func}(int arg)
    {{
        const size_t n = static_cast<size_t>(arg);
        volatile size_t rv = 0;
        for (size_t i = 0; i < n; ++i)
            rv += std::rand() % 2;
    }}
    '''.format(func=name)

    def gen_test(i):
        test_name = '{func}_{i}'.format(func=name, i=i)
        return 'NONIUS_BENCHMARK("{test_name}", [](){{ {func}({i}); }})'.format(
            test_name=test_name, func=name, i=i)

    parts = [header] + [gen_test(i) for i in range(begin, end)]
    return '\n'.join(parts)


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


def __gen_nonius_test_malloc(name, begin, end):
    header = '''
    #include <nonius/nonius.h++>

    static void {func}(int size)
    {{
        std::set<size_t> s;
        for (size_t i = 0; i < size; ++i)
            s.insert(i);
    }}
    '''.format(func=name)

    def gen_test(i):
        test_name = '{func}_{i}'.format(func=name, i=i)
        return 'NONIUS_BENCHMARK("{test_name}", [](){{ {func}({i}); }})'.format(
            test_name=test_name, func=name, i=i)

    parts = [header] + [gen_test(i) for i in range(begin, end)]
    return '\n'.join(parts)


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

NONIUS_TEST_MATH_MICRO  = __gen_nonius_test_math('func_math_micro' , 1, 51)
NONIUS_TEST_MATH_SMALL  = __gen_nonius_test_math('func_math_small' , 501, 551)
NONIUS_TEST_MATH_MEDIUM = __gen_nonius_test_math('func_math_medium', 5001, 5051)
NONIUS_TEST_MATH_LARGE  = __gen_nonius_test_math('func_math_large' , 50001, 50051)

NONIUS_TEST_MALLOC_MICRO  = __gen_nonius_test_malloc('func_malloc_micro' , 1, 51)
NONIUS_TEST_MALLOC_SMALL  = __gen_nonius_test_malloc('func_malloc_small' , 501, 551)
NONIUS_TEST_MALLOC_MEDIUM = __gen_nonius_test_malloc('func_malloc_medium', 5001, 5051)
NONIUS_TEST_MALLOC_LARGE  = __gen_nonius_test_malloc('func_malloc_large' , 50001, 50051)
