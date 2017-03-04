import metrics.utils.backend as backend

from collections import namedtuple


_CompareItemT = namedtuple('CompareItemT', 'name,prev,next,ratio')


def _parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='Utility to compare benchmark results.')
    parser.add_argument('--backend', default=backend.SLTBENCH, choices=backend.ALL)
    parser.add_argument('--format', default='readable', choices=['readable', 'json'])
    parser.add_argument('--file_prev', required=True, help='file with performance results in json format')
    parser.add_argument('--file_next', required=True, help='file with performance results in json format')
    parser.add_argument('--name_prev', required=False, default='prev', help='name of the column with "file_prev" results')
    parser.add_argument('--name_next', required=False, default='next', help='name of the column with "file_next" results')
    return parser.parse_args()


def _create_backend(args):
    if args.backend == backend.SLTBENCH:
        return backend.BackendSLTBench(install_path=None)
    if args.backend == backend.GOOGLEBENCH:
        return backend.BackendGooglebench(install_path=None)
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))


def _compare(r_prev, r_next):
    if len(r_prev) != len(r_next):
        msg = 'Cannot compare, {} functions in prev result and {} in the next one.'.format(
            len(r_prev), len(r_next))
        raise RuntimeError(msg)

    return [_make_compare_item(r_prev[i], r_next[i]) for i in range(len(r_prev))]


def _make_compare_item(i_prev, i_next):
    if i_prev.name != i_next.name:
        msg = 'Cannot compare, no result for {}'.format(i_prev.name)
        raise RuntimeError(msg)

    r = 1
    if i_next.time:
        r = float(i_prev.time) / i_next.time

    return _CompareItemT(name=i_prev.name, prev=i_prev.time, next=i_next.time, ratio=r)


def _report(compare_result, args):
    if args.format == 'readable':
        _report_readable(compare_result, args)
    if args.format == 'json':
        _report_json(compare_result, args)


def _report_json(compare_result, args):
    print('[')
    lines = []
    for item in compare_result:
        lines.append('{{ "name": {}, "{}": {}, "{}": {}, "ratio": {} }}'
            .format(item.name,
                    args.name_prev, item.prev,
                    args.name_next, item.next,
                    item.ratio))
    print(',\n'.join(lines))
    print(']')


def _report_readable(compare_result, args):
    print('{:<30}{:>15}{:>15}{:>8}'
        .format('function', args.name_prev, args.name_next, 'ratio'))
    for item in compare_result:
        print('{:<30}{:>15}{:>15}{:>8.3f}'.
            format(item.name, item.prev, item.next, item.ratio))


def _main():
    args = _parse_args()

    backend = _create_backend(args)

    measure_result_prev = backend.result_parser.parse(args.file_prev)
    measure_result_next = backend.result_parser.parse(args.file_next)

    compare_result = _compare(measure_result_prev, measure_result_next)

    _report(compare_result, args)


if __name__ == "__main__":
    _main()
