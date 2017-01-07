

from collections import namedtuple


_Res = namedtuple('BenchmarkResult', 'name,time')


class PerfResultsParserSLTBench:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:

            def make_name(item):
                if item['arg']:
                    return '{}/{}'.format(item['name'], item['arg'])
                return item['name']

            import json
            root = json.load(f)
            rv = [_Res(name=make_name(x), time=x['time(ns)']) for x in root]
            rv.sort(key=lambda x: x.name)
            return rv


class PerfResultsParserGoogleBench:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:
            import json
            root = json.load(f)['benchmarks']
            rv = [_Res(name=x['name'], time=x['real_time']) for x in root]
            rv.sort(key=lambda x: x.name)
            return rv
