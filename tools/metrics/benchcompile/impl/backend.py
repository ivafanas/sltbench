
import codegen
import dataset
import utils.backend as backends_common


def gen_suite_code(backend, suite, test_id):
    if suite not in backend.suite_to_generator:
        raise RuntimeError('Unsupported suite: {}'.format(suite))
    return backend.suite_to_generator[suite](test_id)


def create(args):
    if args.backend == backends_common.SLTBENCH:
        rv = backends_common.BackendSLTBench(install_path=args.backend_install_path)
        rv.suite_to_generator = {
            dataset.SIMPLE: codegen.gen_sltbench_test_simple,
            dataset.FIXTURE: codegen.gen_sltbench_test_fixture,
            dataset.ARGS: codegen.gen_sltbench_test_args,
            dataset.GENERATOR: codegen.gen_sltbench_test_generator,
            dataset.LAZY_GENERATOR: codegen.gen_sltbench_test_lazy_generator,
            dataset.FIXTURE_ARGS: codegen.gen_sltbench_test_fixture_args,
            dataset.FIXTURE_GENERATOR: codegen.gen_sltbench_test_fixture_generator,
            dataset.FIXTURE_LAZY_GENERATOR: codegen.gen_sltbench_test_fixture_lazy_generator }
        return rv
    if args.backend == backends_common.GOOGLEBENCH:
        rv = backends_common.BackendGooglebench(install_path=args.backend_install_path)
        rv.suite_to_generator = {
            dataset.SIMPLE: codegen.gen_googlebench_test_simple,
            dataset.FIXTURE: codegen.gen_googlebench_test_fixture }
        return rv
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))
