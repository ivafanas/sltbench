import parsers

_SLTBENCH_CPPMAIN = '''
#include <sltbench/Bench.h>
SLTBENCH_MAIN();
'''

_GOOGLEBENCH_CPPMAIN = '''
#include <benchmark/benchmark.h>
BENCHMARK_MAIN();
'''

_NONIUS_CPPMAIN = '''
#define NONIUS_RUNNER
#include <nonius/nonius.h++>
#include <nonius/main.h++>
'''

SLTBENCH = 'sltbench'
GOOGLEBENCH = 'googlebench'
NONIUS = 'nonius'
ALL = [SLTBENCH, GOOGLEBENCH, NONIUS]


class BackendSLTBench:

    def __init__(self, install_path):
        self.install_path = install_path
        self.is_header_only = False
        self.static_lib_name = 'sltbench'
        self.required_static_libs = []
        self.maincpp_code = _SLTBENCH_CPPMAIN
        self.option_reporter = '--reporter=json'
        self.result_parser = parsers.PerfResultsParserSLTBench()


class BackendGooglebench:

    def __init__(self, install_path):
        self.install_path = install_path
        self.is_header_only = False
        self.static_lib_name = 'benchmark'
        self.required_static_libs = ['pthread']
        self.maincpp_code = _GOOGLEBENCH_CPPMAIN
        self.option_reporter = '--benchmark_format=json'
        self.result_parser = parsers.PerfResultsParserGoogleBench()


class BackendNonius:

    def __init__(self, install_path):
        self.install_path = install_path
        self.is_header_only = True
        self.static_lib_name = None
        self.required_static_libs = ['pthread']
        self.maincpp_code = _NONIUS_CPPMAIN
        self.option_reporter = '--reporter=junit'
        self.result_parser = parsers.PerfResultsParserNonius()
