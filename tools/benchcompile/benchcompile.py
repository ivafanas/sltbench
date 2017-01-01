'''
Benchmark for performance tests compilation time
'''

import impl.backend as backend
import impl.benchcc as benchcc
import impl.dataset as dataset
import impl.toolset as toolset


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='benchcompile utility')
    parser.add_argument('--backend', default=backend.SLTBENCH, choices=backend.ALL)
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
    print('compilation time: {} sec'.format(result))


if __name__ == "__main__":
    _main()
