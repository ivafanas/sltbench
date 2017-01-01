'''
Benchmark for performance tests compilation time
'''

import impl.backend as backend
import impl.dataset as dataset
import impl.toolset as toolset

import os
import shutil
import subprocess
import tempfile
import time

# TODO: add HOWTO doc

# TODO: use something prettier
class Object(object):
    pass


def parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='benchcompile utility')
    parser.add_argument('--backend', default=backend.SLTBENCH, choices=backend.ALL)
    parser.add_argument('--backend_install_path', required=True)
    parser.add_argument('--dataset', default=dataset.COMPARABLE, choices=dataset.ALL_INPUT)
    parser.add_argument('--toolset', default=toolset.CLANG, choices=toolset.ALL)
    return parser.parse_args()


def create_bench_context(args):
    context = Object()
    context.backend = backend.create(args)
    context.dataset = dataset.create(args)
    context.toolset = toolset.create(args)
    return context


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
            backend_install_path=backend.install_path,
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
