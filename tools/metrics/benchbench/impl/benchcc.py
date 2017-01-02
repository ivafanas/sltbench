
from utils.cmakegen import gen_cmakelists
from utils.fs import make_temp_dir, print_to_file
from backend import gen_code

import os
import subprocess


def _generate_project(context, path):
    os.chdir(path)

    # build sources
    sources = []
    for size in context.dataset.sizes:
        for kind in context.dataset.kinds:
            filename = 'test_{}_{}.cpp'.format(size, kind)
            print_to_file(filename, gen_code(context.backend, size, kind))
            sources.append(filename)
    print_to_file('main.cpp', context.backend.maincpp_code)
    sources.append('main.cpp')

    # build makefile
    print_to_file('CMakeLists.txt', gen_cmakelists(sources, context.backend))
    subprocess.call('cd {} && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER={} -DCMAKE_CXX_COMPILER={}'
        .format(path, context.toolset.c_compiler, context.toolset.cxx_compiler), shell=True)


def _run_make(context, path):
    subprocess.call(['cd {} && make'.format(path)], shell=True)


def _run_runner(path):
    subprocess.call(['cd {} && ./runner'.format(path)], shell=True)


def benchmark(context):
    # get runner directory and jump into it
    runner_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(runner_dir)

    with make_temp_dir(runner_dir) as temp_dir:
        # create sources, cmake, makefiles etc
        _generate_project(context, temp_dir)

        # build runner_dir
        _run_make(context, temp_dir)

        # measure benchmark time
        import time
        start_make_ts = time.time()
        _run_runner(temp_dir)
        return time.time() - start_make_ts
