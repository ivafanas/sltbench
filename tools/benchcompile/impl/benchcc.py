
import codegen

from contextlib import contextmanager
import os
import subprocess


@contextmanager
def _make_temp_dir(parent_dir):
    # create temp directory
    import tempfile
    temp_dir = tempfile.mkdtemp(dir=parent_dir)

    # return temporary directory
    yield temp_dir

    # cleanup parent directory
    os.chdir(parent_dir)
    import shutil
    shutil.rmtree(temp_dir)


def _generate_project(context, path):
    TESTS_COUNT_PER_SUITE = 50
    backend = context.backend
    toolset = context.toolset
    os.chdir(path)

    # build sources
    sources = []
    for suite in context.dataset:
        for i in range(TESTS_COUNT_PER_SUITE):
            filename = 'test_{}_{}.cpp'.format(suite, i)
            _print_to_file(filename, backend.gen_suite_code(suite, i))
            sources.append(filename)
    _print_to_file('main.cpp', backend.maincpp_code)
    sources.append('main.cpp')

    # build makefile
    _print_to_file('CMakeLists.txt', codegen.gen_cmakelists(sources, backend))
    subprocess.call('cd {} && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER={} -DCMAKE_CXX_COMPILER={}'
        .format(path, toolset.c_compiler, toolset.cxx_compiler), shell=True)


def _run_make(context, path):
    os.chdir(path)
    subprocess.call(['make'], shell=True)


def _print_to_file(filename, content):
    with open(filename, "w") as f:
        f.write(content)


def benchmark(context):
    # get runner directory and jump into it
    runner_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(runner_dir)

    with _make_temp_dir(runner_dir) as temp_dir:
        # create sources, cmake, makefiles etc
        _generate_project(context, temp_dir)

        # measure compilation time
        import time
        start_make_ts = time.time()
        _run_make(context, temp_dir)
        return time.time() - start_make_ts
