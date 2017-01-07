

from collections import namedtuple


_Res = namedtuple('BenchmarkResult', 'name,time')


class PerfResultsParserSLTBench:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:

            def make_name(item):
                return '{}/{}'.format(item['name'], item['arg'])

            import json
            root = json.load(f)
            return [_Res(name=make_name(x), time=x['time(ns)']) for x in root]


class PerfResultsParserGoogleBench:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:
            import json
            root = json.load(f)['benchmarks']
            return [_Res(name=x['name'], time=x['real_time']) for x in root]
