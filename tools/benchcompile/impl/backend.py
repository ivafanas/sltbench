
import codegen
import dataset


SLTBENCH = 'sltbench'
GOOGLEBENCH = 'googlebench'
ALL = [SLTBENCH, GOOGLEBENCH]


class _Backend(object):

    def __init__(self, install_path, static_lib_name, required_static_libs,
                 maincpp_code, suite_to_generator):
        self.install_path = install_path
        self.static_lib_name = static_lib_name
        self.required_static_libs = required_static_libs
        self.maincpp_code = maincpp_code
        self.suite_to_generator = suite_to_generator

    def gen_suite_code(self, suite, test_id):
        if suite not in self.suite_to_generator:
            raise RuntimeError('Unsupported suite: {}'.format(suite))
        return self.suite_to_generator[suite](test_id)


def create(args):
    if args.backend == SLTBENCH:
        return _Backend(
            install_path=args.backend_install_path,
            static_lib_name='sltbench_static',
            required_static_libs=[],
            maincpp_code = codegen.gen_sltbench_cppmain(),
            suite_to_generator = {
                dataset.SIMPLE: codegen.gen_sltbench_test_simple,
                dataset.FIXTURE: codegen.gen_sltbench_test_fixture,
                dataset.ARGS: codegen.gen_sltbench_test_args,
                dataset.GENERATOR: codegen.gen_sltbench_test_generator,
                dataset.LAZY_GENERATOR: codegen.gen_sltbench_test_lazy_generator,
                dataset.FIXTURE_ARGS: codegen.gen_sltbench_test_fixture_args,
                dataset.FIXTURE_GENERATOR: codegen.gen_sltbench_test_fixture_generator,
                dataset.FIXTURE_LAZY_GENERATOR: codegen.gen_sltbench_test_fixture_lazy_generator })
    if args.backend == GOOGLEBENCH:
        return _Backend(
            install_path=args.backend_install_path,
            static_lib_name='benchmark',
            required_static_libs=['pthread'],
            maincpp_code=codegen.gen_googlebench_cppmain(),
            suite_to_generator = {
                dataset.SIMPLE: codegen.gen_googlebench_test_simple,
                dataset.FIXTURE: codegen.gen_googlebench_test_fixture })
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))
