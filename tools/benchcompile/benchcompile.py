'''
Benchmark for performance tests compilation time
'''

import os
import shutil
import subprocess
import tempfile
import time

# TODO: add HOWTO doc
# TODO: split implementation
SUITE_SIMPLE_FUNCTION = 'simple'
SUITE_ARGS = 'args'
SUITE_GENERATOR = 'generator'
SUITE_LAZY_GENERATOR = 'lazy_generator'
SUITE_FIXTURE = 'fixture'
SUITE_FIXTURE_ARGS = 'fixture_args'
SUITE_FIXTURE_GENERATOR = 'fixture_generator'
SUITE_FIXTURE_LAZY_GENERATOR = 'fixture_lazy_generator'
SUITES_ALL = [
    SUITE_SIMPLE_FUNCTION,
    SUITE_FIXTURE,
]
SUITES_EXT = [
    SUITE_SIMPLE_FUNCTION,
    SUITE_ARGS,
    SUITE_GENERATOR,
    SUITE_LAZY_GENERATOR,
    SUITE_FIXTURE,
    SUITE_FIXTURE_ARGS,
    SUITE_FIXTURE_GENERATOR,
    SUITE_FIXTURE_LAZY_GENERATOR
]


TOOLSET_GCC = 'gcc'
TOOLSET_CLANG = 'clang'
TOOLSETS = [TOOLSET_GCC, TOOLSET_CLANG]


BACKEND_SLTBENCH = 'sltbench'
BACKEND_GOOGLEBENCH = 'googlebench'
BACKENDS = [BACKEND_SLTBENCH, BACKEND_GOOGLEBENCH]


class Object(object):
    pass


class SLTBenchBackend:

    def __init__(self):
        pass

    def gen_main_cpp_code(self):
        return '''
            #include <sltbench/Bench.h>

            SLTBENCH_MAIN();
            '''

    def gen_suite_code(self, suite, test_id):
        if suite == SUITE_SIMPLE_FUNCTION:
            return self._gen_suite_code_simple(test_id)
        if suite == SUITE_FIXTURE:
            return self._gen_suite_code_fixture(test_id)
        if suite == SUITE_ARGS:
            return self._gen_suite_code_args(test_id)
        if suite == SUITE_GENERATOR:
            return self._gen_suite_code_generator(test_id)
        if suite == SUITE_LAZY_GENERATOR:
            return self._gen_suite_code_lazy_generator(test_id)
        if suite == SUITE_FIXTURE_ARGS:
            return self._gen_suite_code_fixture_args(test_id)
        if suite == SUITE_FIXTURE_GENERATOR:
            return self._gen_suite_code_fixture_generator(test_id)
        if suite == SUITE_FIXTURE_LAZY_GENERATOR:
            return self._gen_suite_code_fixture_lazy_generator(test_id)
        raise RuntimeError('Unsupported suite: {}'.format(suite))

    def _gen_suite_code_simple(self, test_id):
        func_name = 'SimpleFunction_{}'.format(test_id)
        return '''
            #include <sltbench/Bench.h>
            #include <string>

            static void {func_name}()
            {{
                std::string rv;
                for (size_t i = 0; i < 100000; ++i)
                    rv += "simple function";
            }}
            SLTBENCH_FUNCTION({func_name});
            '''.format(func_name=func_name)

    def _gen_suite_code_fixture(self, test_id):
        func_name = 'FunctionFixt_{}'.format(test_id)
        return '''
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

            void {func_name}(Fixture::Type& fix)
            {{
                std::sort(fix.begin(), fix.end());
            }}
            SLTBENCH_FUNCTION_WITH_FIXTURE({func_name}, Fixture);

            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_args(self, test_id):
        func_name = 'FunctionArgs_{}'.format(test_id)
        return '''
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

            void {func_name}(const Arg& arg)
            {{
                std::string rv;
                for (size_t i = 0; i < arg.n; ++i)
                    rv += arg.src;
            }}

            const std::vector<Arg> string_mult_args{{ {{1, "a"}}, {{2, "b"}} }};
            SLTBENCH_FUNCTION_WITH_ARGS({func_name}, string_mult_args);
            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_generator(self, test_id):
        func_name = 'FunctionGen_{}'.format(test_id)
        return '''
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
                    return{{ {{"knowledge", 100000}}, {{"power", 200000}} }};
                }}
            }};

            std::ostream& operator << (std::ostream& os, const Generator::ArgType& rhs)
            {{
                return os << rhs.n << '/' << rhs.src;
            }}

            void {func_name}(const Generator::ArgType& arg)
            {{
                std::string rv;
                for (size_t i = 0; i < arg.n; ++i)
                    rv += arg.src;
            }}
            SLTBENCH_FUNCTION_WITH_ARGS_GENERATOR({func_name}, Generator);
            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_lazy_generator(self, test_id):
        func_name = 'FunctionLazyGenerator_{}'.format(test_id)
        return '''
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

            void {func_name}(const Generator::ArgType& arg)
            {{
                // let's propose we are going to do something useful computations here
                std::vector<size_t> vec(arg.size, 0);
                for (size_t i = 0; i < arg.ncalls; ++i)
                    std::random_shuffle(vec.begin(), vec.end());
            }}
            SLTBENCH_FUNCTION_WITH_LAZY_ARGS_GENERATOR({func_name}, Generator);
            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_fixture_args(self, test_id):
        func_name = 'FunctionFixtArgs_{}'.format(test_id)
        return '''
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
                Type& SetUp(const Arg& arg)
                {{
                    return fixture_;
                }}
                void TearDown() {{}}
            private:
                Type fixture_;
            }};

            void {func_name}(Fixture::Type& fix, const Arg& arg)
            {{
                // some useful work here based on fixture and arg
            }}
            static const std::vector<Arg> args = {{ {{ 100000, 1 }} }};
            SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS({func_name}, Fixture, args);
            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_fixture_generator(self, test_id):
        func_name = 'FunctionFixtGen_{}'.format(test_id)
        return '''
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

            void {func_name}(Fixture::Type& fix, const Generator::ArgType& arg)
            {{
                // some useful work here based on fixture and arg
                for (size_t i = 0; i < arg.ncalls; ++i)
                    std::random_shuffle(fix.begin(), fix.end());
            }}
            SLTBENCH_FUNCTION_WITH_FIXTURE_AND_ARGS_GENERATOR({func_name}, Fixture, Generator);
            }}
            '''.format(func_name=func_name)

    def _gen_suite_code_fixture_lazy_generator(self, test_id):
        func_name = 'FunctionFixtLazyGen_{}'.format(test_id)
        return '''
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

            void {func_name}(Fixture::Type& fix, const Generator::ArgType& arg)
            {{
                // some useful work here based on fixture and arg
                for (size_t i = 0; i < arg.ncalls; ++i)
                    std::random_shuffle(fix.begin(), fix.end());
            }}
            SLTBENCH_FUNCTION_WITH_FIXTURE_AND_LAZY_ARGS_GENERATOR({func_name}, Fixture, Generator);

            }}
            '''.format(func_name=func_name)

    def static_lib_name(self):
        return 'sltbench_static'

    def required_static_libs(self):
        return []


class GoogleBenchBackend:

    def __init__(self):
        pass

    def gen_main_cpp_code(self):
        return '''
            #include <benchmark/benchmark.h>

            BENCHMARK_MAIN();
            '''

    def gen_suite_code(self, suite, test_id):
        if suite == SUITE_SIMPLE_FUNCTION:
            return self._gen_suite_code_simple(test_id)
        if suite == SUITE_FIXTURE:
            return self._gen_suite_code_fixture(test_id)
        raise RuntimeError('Unsupported suite: {}'.format(suite))

    def _gen_suite_code_simple(self, test_id):
        func_name = 'SimpleFunction_{}'.format(test_id)
        return '''
            #include <benchmark/benchmark.h>

            #include <string>

            static void {func_name}(benchmark::State& state)
            {{
                std::string x = "hello";
                while (state.KeepRunning())
                {{
                    std::string rv;
                    for (size_t i = 0; i < 100000; ++i)
                        rv += "simple function";
                }}
            }}
            BENCHMARK({func_name});
            '''.format(func_name=func_name)

    def _gen_suite_code_fixture(self, test_id):
        func_name = 'FixtFunction_{}'.format(test_id)
        return '''
            #include <benchmark/benchmark.h>

            #include <algorithm>
            #include <vector>

            static void {func_name}(benchmark::State& state)
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
            BENCHMARK({func_name});
            '''.format(func_name=func_name)

    def static_lib_name(self):
        return 'benchmark'

    def required_static_libs(self):
        return ['pthread']


def parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='benchcompile utility')
    parser.add_argument('--backend', default=BACKEND_SLTBENCH, choices=BACKENDS)
    parser.add_argument('--backend_install_path', required=True)
    parser.add_argument('--dataset', default='all', choices=['all', 'extended'] + SUITES_EXT)
    parser.add_argument('--toolset', default=TOOLSET_CLANG, choices=TOOLSETS)
    return parser.parse_args()


def create_bench_context(args):
    context = Object()
    context.backend = create_backend(args)
    context.dataset = create_dataset(args)
    context.toolset = create_toolset(args)
    # TODO: make correct
    context.backend_install_path =args.backend_install_path
    return context


def create_backend(args):
    if args.backend == BACKEND_SLTBENCH:
        return SLTBenchBackend()
    if args.backend == BACKEND_GOOGLEBENCH:
        return GoogleBenchBackend()
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))


def create_dataset(args):
    if args.dataset == 'all':
        return SUITES_ALL
    if args.dataset == 'extended':
        return SUITES_EXT
    return [args.dataset]


def create_toolset(args):
    # TODO: support toolsets
    return args.toolset


def benchmark(context):
    rv = 0.0

    # save runner directory and jump into it
    runner_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(runner_dir)

    # TODO: make context management, try-finally is not correct here
    # create temp directory
    temp_dir = tempfile.mkdtemp(dir=runner_dir)
    try:
        # create sources, cmake, makefiles etc
        generate_project(context, temp_dir)

        # measure compilation time
        start_make_ts = time.time()
        run_make(context, temp_dir)
        rv = time.time() - start_make_ts
    finally:
        # cleanup temp directory
        os.chdir(runner_dir)
        shutil.rmtree(temp_dir)

    return rv


def generate_project(context, path):
    FUNCTIONS_COUNT_PER_SUITE = 50
    backend = context.backend

    os.chdir(path)

    sources = []

    # build main
    print_to_file('main.cpp', backend.gen_main_cpp_code())
    sources.append('main.cpp')

    # build sources
    for suite in context.dataset:
        for i in range(FUNCTIONS_COUNT_PER_SUITE):
            filename = 'test_{}_{}.cpp'.format(suite, i)
            print_to_file(filename, backend.gen_suite_code(suite, i))
            sources.append(filename)

    # build cmake
    # TODO: support toolsets
    cmake_content = '''
        cmake_minimum_required (VERSION 2.8.0)

        project (benchcompile)

        # setup env
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} -std=c++11")

        # sources
        set(BC_SOURCES {sources_list})

        # include sltbench
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
            backend_install_path=context.backend_install_path,
            backend_static_lib=backend.static_lib_name(),
            required_static_libs=' '.join(backend.required_static_libs()))
    print_to_file('CMakeLists.txt', cmake_content)
    subprocess.call('cd {} && cmake -DCMAKE_BUILD_TYPE=Release'.format(path),
        shell=True)


def run_make(context, path):
    os.chdir(path)
    subprocess.call(['make'], shell=True)


def print_to_file(filename, content):
    with open(filename, "w") as f:
        f.write(content)


def main():
    # parse command line
    args = parse_args()

    # build benchmark context
    context = create_bench_context(args)

    # measure compilation time
    bench_result = benchmark(context)

    # print result
    print('compilation time: {} sec'.format(bench_result))


if __name__ == "__main__":
    main()
