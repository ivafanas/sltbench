
from utils.cmakegen import gen_cmakelists
from utils.fs import make_temp_dir, print_to_file
from backend import gen_code

from collections import namedtuple
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


def _run_runner(backend, path):
    outfile = os.path.abspath('{path}/output.json'.format(path=path))
    command = 'cd {path} && ./runner {options} > {outfile}'.format(
        path=path,
        options=backend.option_json_reporter,
        outfile=outfile)

    import time
    start_ts = time.time()
    subprocess.call([command], shell=True)
    final_ts = time.time()

    RT = namedtuple('_run_runner_res', 'result,time')
    return RT(
        result=backend.result_parser.parse(outfile),
        time=final_ts - start_ts)


def _collect_stat(rr_results):
    bench_results = [x.result for x in rr_results]
    func_to_values = {}
    for bench_res in bench_results:
        for func_res in bench_res:
            if func_res.name not in func_to_values:
                func_to_values[func_res.name] = [func_res.time]
            else:
                func_to_values[func_res.name].append(func_res.time)

    ItemT = namedtuple('FunctionStat', 'name,avr,err')
    items = []
    mean_err = 0.0
    for name, values in func_to_values.iteritems():
        f_min = min(values)
        f_max = max(values)
        f_avr = sum(values) / len(values)
        f_err = 0
        if f_avr:
            f_err = max(
                float(f_avr - f_min) / f_avr,
                float(f_max - f_avr) / f_avr)
        mean_err += f_err
        items.append(ItemT(name=name, avr=f_avr, err=f_err))
    mean_err /= len(items)

    RT = namedtuple('_collect_stat_res', 'functions,mean_err,bench_time')
    return RT(
        functions=items,
        mean_err=mean_err,
        bench_time=min([x.time for x in rr_results]))


def benchmark(context):
    # get runner directory and jump into it
    runner_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(runner_dir)

    with make_temp_dir(runner_dir) as temp_dir:
        # create sources, cmake, makefiles etc
        _generate_project(context, temp_dir)

        # build runner
        _run_make(context, temp_dir)

        # run benchmark several times
        rr_results = []
        n_runs = 4
        for i in range(n_runs):
            print('run {} of {}'.format(i + 1, n_runs))
            rr_results.append(_run_runner(context.backend, temp_dir))

        # collect statistics
        return _collect_stat(rr_results)
