'''
Benchmark for performance tests compilation time
'''

from utils.backend import SLTBENCH as BACKEND_SLTBENCH
from utils.backend import ALL as BACKENDS_ALL
import impl.backend as backend
import impl.benchcc as benchcc
import impl.dataset as dataset
import utils.toolset as toolset


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='Utility to measure benchmark compilation time.')
    parser.add_argument('--backend', default=BACKEND_SLTBENCH, choices=BACKENDS_ALL, help='benchmark framework to test. Default is "sltbench".')
    parser.add_argument('--backend_install_path', required=True, help='path to installed backend with headers and static libs.')
    parser.add_argument('--dataset', default='comparable', choices=dataset.ALL_INPUT, help='testing dataset. sltbench supports all datasets. googlebench supports "simple" and "fixture". nonius supports "simple". Default is "comparable".')
    parser.add_argument('--toolset', default=toolset.CLANG, choices=toolset.ALL, help='compiler tools collection. Default is "clang".')
    return parser.parse_args()


def _create_context(args):
    from collections import namedtuple
    return namedtuple('Context', 'backend,dataset,toolset')(
        backend=backend.create(args),
        dataset=dataset.create(args),
        toolset=toolset.create(args))


def _main():
    # parse command line
    args = _parse_args()

    # build benchmark context
    context = _create_context(args)

    # measure compilation time
    result = benchcc.benchmark(context)

    # print result
    print('{{ "compilation_time_sec": {} }}'.format(result))


if __name__ == "__main__":
    _main()
