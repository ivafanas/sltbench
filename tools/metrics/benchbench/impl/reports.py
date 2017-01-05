
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
    print(json.dumps(res, sort_keys=True, indent=2))


def _report_csv(result, options):
    print('bench_time_sec,{}'.format(result.bench_time))
    print('mean_err_percent,{}'.format(result.mean_err))
    if options.moreinfo:
        for f in result.functions:
            print('{},{},{}'.format(f.name, f.avr, f.err))


def _report_readable(result, options):
    print('bench_time_sec: {}'.format(result.bench_time))
    print('mean_err_percent: {}'.format(result.mean_err))
    if options.moreinfo:
        for f in result.functions:
            print('{:<30}{:>15}{:>8.4f}'.format(f.name, f.avr, f.err))


def report(result, options):
    if options.fmt == FMT_JSON:
        _report_json(result, options)
    if options.fmt == FMT_CSV:
        _report_csv(result, options)
    if options.fmt == FMT_READABLE:
        _report_readable(result, options)


def create_options(args):
    from collections import namedtuple
    RT = namedtuple('ReportOptions', 'moreinfo,fmt')
    return RT(
        moreinfo=args.moreinfo is not None,
        fmt=args.report_format)
