
_SLTBENCH_CPPMAIN = '''
#include <sltbench/Bench.h>
SLTBENCH_MAIN();
'''

_SLTBENCH_TEST_TMPL_SIMPLE = '''
#include <sltbench/Bench.h>
#include <string>

static void simple_{uid}()
{{
    std::string rv;
    for (size_t i = 0; i < 100000; ++i)
        rv += "simple function";
}}
SLTBENCH_FUNCTION(simple_{uid});
'''

_SLTBENCH_TEST_TMPL_ARGS = '''
#include <sltbench/Bench.h>
#include <ostream>
#include <string>

namespace {{

struct Arg
{{
    size_t n;
    std::string src;
}};

std::ostream& operator << (std::ostream& oss, const Arg& rhs)
{{
    return oss << rhs.n << '/' << rhs.src;
}}

void func_args_{uid}(const Arg& arg)
{{
    std::string rv;
    for (size_t i = 0; i < arg.n; ++i)
        rv += arg.src;
}}

const std::vector<Arg> string_mult_args{{ {{1, "a"}}, {{2, "b"}} }};
SLTBENCH_FUNCTION_WITH_ARGS(func_args_{uid}, string_mult_args);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <vector>

namespace {{

class Fixture
{{
public:
    typedef std::vector<size_t> Type;
    Fixture() {{}}
    Type& SetUp() {{ fixture_.resize(1000, 0); }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_{uid}(Fixture::Type& fix)
{{
    std::sort(fix.begin(), fix.end());
}}
SLTBENCH_FUNCTION_WITH_FIXTURE(func_{uid}, Fixture);
}}
'''

_SLTBENCH_TEST_TMPL_GENERATOR = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

namespace {{

class Generator
{{
public:
    struct ArgType
    {{
        std::string src;
        size_t n;
    }};

    Generator() {{}}

    std::vector<ArgType> Generate(int argc, char **argv)
    {{
        return{{ {{"a", 1}}, {{"b", 2}} }};
    }}
}};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{{
    return os << rhs.n << '/' << rhs.src;
}}

void func_gen_{uid}(const Generator::ArgType& arg)
{{
    std::string rv;
    for (size_t i = 0; i < arg.n; ++i)
        rv += arg.src;
}}
SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(func_gen_{uid}, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_LAZY_GENERATOR = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <ostream>
#include <vector>

namespace {{

class Generator
{{
public:
    struct ArgType
    {{
        size_t size;
        size_t ncalls;
    }};

    Generator(int, char **) {{}}

    ArgType Generate()
    {{
        if (generated_count_ >= 3)
            throw sltbench::StopGenerationException();

        ++generated_count_;

        // the only instance of ArgType is in the memory during measurement
        return{{ generated_count_ * 100000, generated_count_ }};
    }}

private:
    size_t generated_count_ = 0;
}};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{{
    return os << rhs.size << '/' << rhs.ncalls;
}}

void func_lazygen_{uid}(const Generator::ArgType& arg)
{{
    // let's propose we are going to do something useful computations here
    std::vector<size_t> vec(arg.size, 0);
    for (size_t i = 0; i < arg.ncalls; ++i)
        std::random_shuffle(vec.begin(), vec.end());
}}
SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func_lazygen_{uid}, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_ARGS = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <ostream>
#include <vector>

namespace {{

struct Arg
{{
    size_t size;
    size_t ncalls;
}};

std::ostream& operator << (std::ostream& oss, const Arg& arg)
{{
    return oss << arg.size << '/' << arg.ncalls;
}}

class Fixture
{{
public:
    typedef std::vector<size_t> Type;
    Fixture() {{}}
    Type& SetUp(const Arg& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_fixargs_{uid}(Fixture::Type& fix, const Arg& arg)
{{
}}
static const std::vector<Arg> args = {{ {{ 100000, 1 }} }};
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(func_fixargs_{uid}, Fixture, args);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_GENERATOR = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <ostream>
#include <vector>

namespace {{

class Generator
{{
public:
    struct ArgType
    {{
        size_t size;
        size_t ncalls;
    }};

    Generator() {{}}

    std::vector<ArgType> Generate(int argc, char **argv)
    {{
        return{{ {{100000, 10}}, {{200000, 20}} }};
    }}
}};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{{
    return os << rhs.ncalls << '/' << rhs.size;
}}

class Fixture
{{
public:
    typedef std::vector<size_t> Type;
    Fixture() {{}}
    Type& SetUp(const Generator::ArgType& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_fixgen_{uid}(Fixture::Type& fix, const Generator::ArgType& arg)
{{
    // some useful work here based on fixture and arg
    for (size_t i = 0; i < arg.ncalls; ++i)
        std::random_shuffle(fix.begin(), fix.end());
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func_fixgen_{uid}, Fixture, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_LAZY_GENERATOR = '''
#include <sltbench/Bench.h>
#include <algorithm>
#include <ostream>
#include <vector>

namespace {{

class Generator
{{
public:
    struct ArgType
    {{
        size_t size;
        size_t ncalls;
    }};

    Generator(int, char **) {{}}

    ArgType Generate()
    {{
        if (generated_count_ >= 3)
            throw sltbench::StopGenerationException();

        ++generated_count_;

        // the only instance of ArgType is in the memory during measurement
        return{{generated_count_ * 100000, generated_count_}};
    }}

private:
    size_t generated_count_ = 0;
}};

std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
{{
    return os << rhs.size << '/' << rhs.ncalls;
}}

class Fixture
{{
public:
    typedef std::vector<size_t> Type;
    Fixture() {{}}
    Type& SetUp(const Generator::ArgType& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_fix_lazygen_{uid}(Fixture::Type& fix, const Generator::ArgType& arg)
{{
    // some useful work here based on fixture and arg
    for (size_t i = 0; i < arg.ncalls; ++i)
        std::random_shuffle(fix.begin(), fix.end());
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func_fix_lazygen_{uid}, Fixture, Generator);
}}
'''

_GOOGLEBENCH_CPPMAIN = '''
#include <benchmark/benchmark.h>
BENCHMARK_MAIN();
'''

_GOOGLEBENCH_TEST_TMPL_SIMPLE = '''
#include <benchmark/benchmark.h>

#include <string>

static void simple_{uid}(benchmark::State& state)
{{
    std::string x = "hello";
    while (state.KeepRunning())
    {{
        std::string rv;
        for (size_t i = 0; i < 100000; ++i)
            rv += "simple function";
    }}
}}
BENCHMARK(simple_{uid});
'''

_GOOGLEBENCH_TEST_TMPL_FIXTURE = '''
#include <benchmark/benchmark.h>

#include <algorithm>
#include <vector>

static void func_fix_{uid}(benchmark::State& state)
{{
    std::vector<size_t> v;
    while (state.KeepRunning())
    {{
        state.PauseTiming();
        v.resize(1000, 0);
        state.ResumeTiming();

        std::sort(v.begin(), v.end());
    }}
}}
BENCHMARK(func_fix_{uid});
'''


def gen_sltbench_cppmain():
    return _SLTBENCH_CPPMAIN


def gen_sltbench_test_simple(uid):
    return _SLTBENCH_TEST_TMPL_SIMPLE.format(uid=uid)


def gen_sltbench_test_fixture(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE.format(uid=uid)


def gen_sltbench_test_args(uid):
    return _SLTBENCH_TEST_TMPL_ARGS.format(uid=uid)


def gen_sltbench_test_generator(uid):
    return _SLTBENCH_TEST_TMPL_GENERATOR.format(uid=uid)


def gen_sltbench_test_lazy_generator(uid):
    return _SLTBENCH_TEST_TMPL_LAZY_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture_args(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_ARGS.format(uid=uid)


def gen_sltbench_test_fixture_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture_lazy_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_LAZY_GENERATOR.format(uid=uid)


def gen_googlebench_cppmain():
    return _GOOGLEBENCH_CPPMAIN


def gen_googlebench_test_simple(uid):
    return _GOOGLEBENCH_TEST_TMPL_SIMPLE.format(uid=uid)


def gen_googlebench_test_fixture(uid):
    return _GOOGLEBENCH_TEST_TMPL_FIXTURE.format(uid=uid)


def gen_cmakelists(sources, backend):
    return '''
        cmake_minimum_required (VERSION 2.8.0)

        project (benchcompile)

        # setup env
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} -std=c++11")

        # sources
        set(BC_SOURCES {sources_list})

        # include backend
        include_directories({backend_install_path}/include)
        link_directories({backend_install_path}/lib)

        # build exe-file
        add_executable(benchcompile ${{BC_SOURCES}})

        # link with sltbench
        target_link_libraries(benchcompile LINK_PUBLIC
            {backend_static_lib}
            {required_static_libs})
        '''.format(
            sources_list=' '.join(sources),
            backend_install_path=backend.install_path,
            backend_static_lib=backend.static_lib_name(),
            required_static_libs=' '.join(backend.required_static_libs()))
