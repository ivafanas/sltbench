from collections import namedtuple
from xml.dom import minidom

import json


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

            root = json.load(f)
            rv = [_Res(name=make_name(x), time=x['time(ns)']) for x in root]
            rv.sort(key=lambda x: x.name)
            return rv


class PerfResultsParserGoogleBench:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:
            root = json.load(f)['benchmarks']
            rv = [_Res(name=x['name'], time=x['real_time']) for x in root]
            rv.sort(key=lambda x: x.name)
            return rv


class PerfResultsParserNonius:

    def __init__(self):
        pass

    def parse(self, file):
        with open(file, 'r') as f:
            def make_res(i):
                return _Res(name=i.attributes['name'].value,
                            time=int(float(i.attributes['time'].value) * 1e9))

            doc = minidom.parse(f)
            rv = [make_res(x) for x in doc.getElementsByTagName('testcase')]
            rv.sort(key=lambda x: x.name)
            return rv
