
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
            dataset.ARGS: codegen.gen_sltbench_test_args,
            dataset.GENERATOR: codegen.gen_sltbench_test_generator,
            dataset.LAZY_GENERATOR: codegen.gen_sltbench_test_lazy_generator,
            dataset.FIXTURE: codegen.gen_sltbench_test_fixture,
            dataset.FIXTURE_ARGS: codegen.gen_sltbench_test_fixture_args,
            dataset.FIXTURE_GENERATOR: codegen.gen_sltbench_test_fixture_generator,
            dataset.FIXTURE_LAZY_GENERATOR: codegen.gen_sltbench_test_fixture_lazy_generator,
            dataset.FIXTURE_BUILDER: codegen.gen_sltbench_test_fixture_builder,
            dataset.FIXTURE_BUILDER_ARGS: codegen.gen_sltbench_test_fixture_builder_args,
            dataset.FIXTURE_BUILDER_GENERATOR: codegen.gen_sltbench_test_fixture_builder_generator,
            dataset.FIXTURE_BUILDER_LAZY_GENERATOR: codegen.gen_sltbench_test_fixture_builder_lazy_generator}
        return rv
    if args.backend == backends_common.GOOGLEBENCH:
        rv = backends_common.BackendGooglebench(install_path=args.backend_install_path)
        rv.suite_to_generator = {
            dataset.SIMPLE: codegen.gen_googlebench_test_simple,
            dataset.FIXTURE: codegen.gen_googlebench_test_fixture }
        return rv
    if args.backend == backends_common.NONIUS:
        rv = backends_common.BackendNonius(install_path=args.backend_install_path)
        rv.suite_to_generator = {
            dataset.SIMPLE: codegen.gen_nonius_test_simple }
        return rv
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))
