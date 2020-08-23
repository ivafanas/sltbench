
_SLTBENCH_TEST_TMPL_SIMPLE = '''
#include <sltbench/BenchCore.h>

static void simple_{uid}()
{{
}}
SLTBENCH_FUNCTION(simple_{uid});
'''

_SLTBENCH_TEST_TMPL_ARGS = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

void func_args_{uid}(const int& arg)
{{
}}

const std::vector<int> string_mult_args{{ 1, 2 }};
SLTBENCH_FUNCTION_WITH_ARGS(func_args_{uid}, string_mult_args);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE = '''
#include <sltbench/Bench.h>

namespace {{

class Fixture
{{
public:
    using Type = int;
    Type& SetUp() {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_{uid}(Fixture::Type& fix)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE(func_{uid}, Fixture);
}}
'''

_SLTBENCH_TEST_TMPL_GENERATOR = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    std::vector<ArgType> Generate(int argc, char **argv)
    {{
        return{{ 1, 2 }};
    }}
}};

void func_gen_{uid}(const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR(func_gen_{uid}, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_LAZY_GENERATOR = '''
#include <sltbench/Bench.h>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    Generator(int, char **) {{}}

    ArgType Generate()
    {{
        if (generated_count_ >= 3)
            throw sltbench::StopGenerationException();

        ++generated_count_;

        // the only instance of ArgType is in the memory during measurement
        return 42;
    }}

private:
    size_t generated_count_ = 0;
}};

void func_lazygen_{uid}(const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR(func_lazygen_{uid}, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_ARGS = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

using Arg = int;

class Fixture
{{
public:
    using Type = int;
    Type& SetUp(const Arg& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_ = 0;
}};

void func_fixargs_{uid}(Fixture::Type& fix, const Arg& arg)
{{
}}
static const std::vector<Arg> args = {{ 1, 2 }};
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS(func_fixargs_{uid}, Fixture, args);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_GENERATOR = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    std::vector<ArgType> Generate(int argc, char **argv)
    {{
        return{{ 1, 2 }};
    }}
}};

class Fixture
{{
public:
    using Type = int;
    Type& SetUp(const Generator::ArgType& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_ = 0;
}};

void func_fixgen_{uid}(Fixture::Type& fix, const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR(func_fixgen_{uid}, Fixture, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_LAZY_GENERATOR = '''
#include <sltbench/Bench.h>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    Generator(int, char **) {{}}

    ArgType Generate()
    {{
        if (generated_count_ >= 3)
            throw sltbench::StopGenerationException();

        ++generated_count_;

        return 42;
    }}

private:
    size_t generated_count_ = 0;
}};

class Fixture
{{
public:
    using Type = int;
    Type& SetUp(const Generator::ArgType& arg) {{ return fixture_; }}
    void TearDown() {{}}
private:
    Type fixture_;
}};

void func_fix_lazygen_{uid}(Fixture::Type& fix, const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR(func_fix_lazygen_{uid}, Fixture, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_BUILDER = '''
#include <sltbench/Bench.h>

namespace {{

int make_fixture()
{{
    return 42;
}}

void func_fb_{uid}(int& fix)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER(func_fb_{uid}, make_fixture);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_ARGS = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

using Arg = int;

int make_fixture(const Arg&)
{{
    return 42;
}}

void func_fb_a_{uid}(int&, const Arg&)
{{
}}
const std::vector<Arg> args = {{ 1, 2 }};
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS(func_fb_a_{uid}, make_fixture, args);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_GENERATOR = '''
#include <sltbench/Bench.h>
#include <vector>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    std::vector<ArgType> Generate(int argc, char **argv)
    {{
        return{{ 1, 2 }};
    }}
}};

int make_fixture(const Generator::ArgType&)
{{
    return {{ }};
}}

void func_fb_g_{uid}(int& fix, const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_ARGS_GENERATOR(func_fb_g_{uid}, make_fixture, Generator);
}}
'''

_SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_LAZY_GENERATOR = '''
#include <sltbench/Bench.h>

namespace {{

class Generator
{{
public:
    using ArgType = int;

    Generator(int, char **) {{}}

    ArgType Generate()
    {{
        if (generated_count_ >= 3)
            throw sltbench::StopGenerationException();

        ++generated_count_;

        return 42;
    }}

private:
    size_t generated_count_ = 0;
}};

int make_fixture(const Generator::ArgType&)
{{
    return {{ }};
}}

void func_fb_lag_{uid}(int& fix, const Generator::ArgType& arg)
{{
}}
SLTBENCH_FUNCTION_WITH_FIXTURE_BUILDER_AND_LAZY_ARGS_GENERATOR(func_fb_lag_{uid}, make_fixture, Generator);
}}
'''

_GOOGLEBENCH_TEST_TMPL_SIMPLE = '''
#include <benchmark/benchmark.h>

static void simple_{uid}(benchmark::State& state)
{{
    while (state.KeepRunning())
    {{
    }}
}}
BENCHMARK(simple_{uid});
'''

_GOOGLEBENCH_TEST_TMPL_FIXTURE = '''
#include <benchmark/benchmark.h>

static void func_fix_{uid}(benchmark::State& state)
{{
    while (state.KeepRunning())
    {{
        state.PauseTiming();
        state.ResumeTiming();
    }}
}}
BENCHMARK(func_fix_{uid});
'''

_NONIUS_TEST_TMPL_SIMPLE = '''
#include <nonius/nonius.h++>

static void simple_{uid}()
{{
}}
NONIUS_BENCHMARK("simple_{uid}", [](){{ simple_{uid}(); }})
'''


def gen_sltbench_test_simple(uid):
    return _SLTBENCH_TEST_TMPL_SIMPLE.format(uid=uid)


def gen_sltbench_test_args(uid):
    return _SLTBENCH_TEST_TMPL_ARGS.format(uid=uid)


def gen_sltbench_test_generator(uid):
    return _SLTBENCH_TEST_TMPL_GENERATOR.format(uid=uid)


def gen_sltbench_test_lazy_generator(uid):
    return _SLTBENCH_TEST_TMPL_LAZY_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE.format(uid=uid)


def gen_sltbench_test_fixture_args(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_ARGS.format(uid=uid)


def gen_sltbench_test_fixture_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture_lazy_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_LAZY_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture_builder(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_BUILDER.format(uid=uid)


def gen_sltbench_test_fixture_builder_args(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_ARGS.format(uid=uid)


def gen_sltbench_test_fixture_builder_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_GENERATOR.format(uid=uid)


def gen_sltbench_test_fixture_builder_lazy_generator(uid):
    return _SLTBENCH_TEST_TMPL_FIXTURE_BUILDER_LAZY_GENERATOR.format(uid=uid)


def gen_googlebench_test_simple(uid):
    return _GOOGLEBENCH_TEST_TMPL_SIMPLE.format(uid=uid)


def gen_googlebench_test_fixture(uid):
    return _GOOGLEBENCH_TEST_TMPL_FIXTURE.format(uid=uid)


def gen_nonius_test_simple(uid):
    return _NONIUS_TEST_TMPL_SIMPLE.format(uid=uid)
