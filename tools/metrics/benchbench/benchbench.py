# TODO: measure dispersion
# TODO: add HOWTO doc

from utils.backend import SLTBENCH as BACKEND_SLTBENCH
from utils.backend import ALL as BACKEND_ALL

import impl.backend as backend
import impl.benchcc as benchcc
import impl.dataset as dataset
import utils.toolset as toolset


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='benchmark for benchmark utility.')
    parser.add_argument('--backend', default=BACKEND_SLTBENCH, choices=BACKEND_ALL)
    parser.add_argument('--backend_install_path', required=True)
    parser.add_argument('--sizes', default='all', choices=dataset.SIZES_INPUT_ALL)
    parser.add_argument('--kinds', default='all', choices=dataset.KINDS_INPUT_ALL)
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
    print('{{ "benchmark_time_sec": {} }}'.format(result))


if __name__ == "__main__":
    _main()
