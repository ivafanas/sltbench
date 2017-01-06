
from utils.fs import print_to_file, make_abspath_from_cwd


FMT_JSON = 'json'
FMT_CSV = 'csv'
FMT_READABLE = 'readable'
FMT_ALL = [FMT_JSON, FMT_CSV, FMT_READABLE]


def _report_json(result, options):
    import json
    res = {}
    res['mean_err_percent'] = result.mean_err
    res['bench_time_sec'] = result.bench_time
    if options.moreinfo:
        res['functions'] = []
        for func in result.functions:
            item = {'name': func.name, 'avr': func.avr, 'err': func.err}
            res['functions'].append(item)
    return json.dumps(res, sort_keys=True, indent=2)


def _report_csv(result, options):
    lines = []
    lines.append('bench_time_sec,{}'.format(result.bench_time))
    lines.append('mean_err_percent,{}'.format(result.mean_err))
    if options.moreinfo:
        for f in result.functions:
            lines.append('{},{},{}'.format(f.name, f.avr, f.err))
    return '\n'.join(lines)


def _report_readable(result, options):
    lines = []
    lines.append('bench_time_sec: {}'.format(result.bench_time))
    lines.append('mean_err_percent: {}'.format(result.mean_err))
    if options.moreinfo:
        for f in result.functions:
            lines.append('{:<30}{:>15}{:>8.4f}'.format(f.name, f.avr, f.err))
    return '\n'.join(lines)


def report(result, options):
    content = ''
    if options.fmt == FMT_JSON:
        content = _report_json(result, options)
    if options.fmt == FMT_CSV:
        content = _report_csv(result, options)
    if options.fmt == FMT_READABLE:
        content = _report_readable(result, options)

    print(content)
    if options.outfile:
        print_to_file(options.outfile, content)


def create_options(args):

    outfile = None
    if args.outfile:
        outfile = make_abspath_from_cwd(args.outfile)

    from collections import namedtuple
    RT = namedtuple('ReportOptions', 'moreinfo,fmt,outfile')
    return RT(
        moreinfo=args.moreinfo is not None,
        fmt=args.report_format,
        outfile=outfile)
