from utils.backend import SLTBENCH as BACKEND_SLTBENCH
from utils.backend import ALL as BACKEND_ALL

import impl.backend as backend
import impl.benchcc as benchcc
import impl.dataset as dataset
import impl.reports as reports
import utils.toolset as toolset


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='Utility to measure benchmark time and error.')
    parser.add_argument('--backend', default=BACKEND_SLTBENCH, choices=BACKEND_ALL, help='benchmark framework to test. Default is "sltbench"')
    parser.add_argument('--backend_install_path', required=True, help='path to installed backend with headers and static libs.')
    parser.add_argument('--sizes', default='all', choices=dataset.SIZES_INPUT_ALL, help='set of problems sizes for testing. Default is "all".')
    parser.add_argument('--kinds', default='all', choices=dataset.KINDS_INPUT_ALL, help='set of test kinds. Default is "all".')
    parser.add_argument('--toolset', default=toolset.CLANG, choices=toolset.ALL, help='compiler tools collection. Default is "clang".')
    parser.add_argument('--moreinfo', nargs='*', help='produce detailed output.')
    parser.add_argument('--report_format', default=reports.FMT_READABLE, choices=reports.FMT_ALL, help='output format. Default is "readable".')
    parser.add_argument('--outfile', default=None, help='output file to save report. Output will not be saved to file by default.')
    parser.add_argument('--runcount', default=4, type=int, help='count of benchmark runs to collect statistics. Default is 4.')
    parser.add_argument('--pincpu', default=None, help='input arguments to taskset to pin benchmark to the cpu. No pinning by default because it is definitely machine-specific.')
    return parser.parse_args()


def _create_context(args):
    from collections import namedtuple
    return namedtuple('Context', 'backend,dataset,toolset,reports,benchcc')(
        backend=backend.create(args),
        dataset=dataset.create(args),
        toolset=toolset.create(args),
        reports=reports.create_options(args),
        benchcc=benchcc.create_options(args))


def _main():
    args = _parse_args()

    context = _create_context(args)

    result = benchcc.benchmark(context)

    reports.report(result, context.reports)


if __name__ == "__main__":
    _main()
