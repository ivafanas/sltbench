'''
Benchmark for performance tests compilation time
'''

# TODO: update README, it is not runnable more from its folder
from utils.backend import SLTBENCH as BACKEND_SLTBENCH
from utils.backend import ALL as BACKENDS_ALL
import impl.backend as backend
import impl.benchcc as benchcc
import impl.dataset as dataset
import utils.toolset as toolset


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='benchcompile utility')
    parser.add_argument('--backend', default=BACKEND_SLTBENCH, choices=BACKENDS_ALL)
    parser.add_argument('--backend_install_path', required=True)
    parser.add_argument('--dataset', default='comparable', choices=dataset.ALL_INPUT)
    parser.add_argument('--toolset', default=toolset.CLANG, choices=toolset.ALL)
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
