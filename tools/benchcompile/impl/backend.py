
import codegen
import dataset


SLTBENCH = 'sltbench'
GOOGLEBENCH = 'googlebench'
ALL = [SLTBENCH, GOOGLEBENCH]


class _SLTBenchBackend:

    def __init__(self, install_path):
        self.install_path = install_path

    def gen_main_cpp_code(self):
        return codegen.gen_sltbench_cppmain()

    def gen_suite_code(self, suite, test_id):
        if suite == dataset.SIMPLE:
            return codegen.gen_sltbench_test_simple(test_id)
        if suite == dataset.FIXTURE:
            return codegen.gen_sltbench_test_fixture(test_id)
        if suite == dataset.ARGS:
            return codegen.gen_sltbench_test_args(test_id)
        if suite == dataset.GENERATOR:
            return codegen.gen_sltbench_test_generator(test_id)
        if suite == dataset.LAZY_GENERATOR:
            return codegen.gen_sltbench_test_lazy_generator(test_id)
        if suite == dataset.FIXTURE_ARGS:
            return codegen.gen_sltbench_test_fixture_args(test_id)
        if suite == dataset.FIXTURE_GENERATOR:
            return codegen.gen_sltbench_test_fixture_generator(test_id)
        if suite == dataset.FIXTURE_LAZY_GENERATOR:
            return codegen.gen_sltbench_test_fixture_lazy_generator(test_id)
        raise RuntimeError('Unsupported suite: {}'.format(suite))

    def static_lib_name(self):
        return 'sltbench_static'

    def required_static_libs(self):
        return []


class _GoogleBenchBackend:

    def __init__(self, install_path):
        self.install_path = install_path

    def gen_main_cpp_code(self):
        return codegen.gen_googlebench_cppmain()

    def gen_suite_code(self, suite, test_id):
        if suite == dataset.SIMPLE:
            return codegen.gen_googlebench_test_simple(test_id)
        if suite == dataset.FIXTURE:
            return codegen.gen_googlebench_test_fixture(test_id)
        raise RuntimeError('Unsupported suite: {}'.format(suite))

    def static_lib_name(self):
        return 'benchmark'

    def required_static_libs(self):
        return ['pthread']


def create(args):
    if args.backend == SLTBENCH:
        return _SLTBenchBackend(install_path=args.backend_install_path)
    if args.backend == GOOGLEBENCH:
        return _GoogleBenchBackend(install_path=args.backend_install_path)
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))
