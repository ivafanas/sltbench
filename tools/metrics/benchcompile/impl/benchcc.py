
from utils.cmakegen import gen_cmakelists
from utils.fs import make_temp_dir, print_to_file
from backend import gen_suite_code

import os
import subprocess


def _generate_project(context, path):
    TESTS_COUNT_PER_SUITE = 50
    backend = context.backend
    toolset = context.toolset
    build_system = context.build_system
    os.chdir(path)

    # build sources
    sources = []
    for suite in context.dataset:
        for i in range(TESTS_COUNT_PER_SUITE):
            filename = 'test_{}_{}.cpp'.format(suite, i)
            print_to_file(filename, gen_suite_code(backend, suite, i))
            sources.append(filename)
    print_to_file('main.cpp', backend.maincpp_code)
    sources.append('main.cpp')

    # build makefile
    print_to_file('CMakeLists.txt', gen_cmakelists(sources, backend))
    cmake_options = [
        '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_C_COMPILER={}'.format(toolset.c_compiler),
        '-DCMAKE_CXX_COMPILER={}'.format(toolset.cxx_compiler),
        '-G "{}"'.format(build_system.cmake_generator)]
    subprocess.call(
        'cd {} && cmake {}'.format(path, ' '.join(cmake_options)), shell=True)


def _run_make(context, path):
    os.chdir(path)
    subprocess.call([context.build_system.build_command], shell=True)


def benchmark(context):
    # get runner directory and jump into it
    runner_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(runner_dir)

    with make_temp_dir(runner_dir) as temp_dir:
        # create sources, cmake, makefiles etc
        _generate_project(context, temp_dir)

        # measure compilation time
        import time
        start_make_ts = time.time()
        _run_make(context, temp_dir)
        return time.time() - start_make_ts
