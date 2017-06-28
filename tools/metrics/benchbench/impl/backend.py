
import codegen
import dataset
import utils.backend as backends_common


def _casekey(size, kind):
    return size + '_' + kind


def gen_code(backend, size, kind):
    key = _casekey(size, kind)
    if key in backend.case_to_code:
        return backend.case_to_code[key]
    raise RuntimeError('Unrecognized configuration: {}, {}'.format(size, kind))


def create(args):
    if args.backend == backends_common.SLTBENCH:
        rv = backends_common.BackendSLTBench(install_path=args.backend_install_path)
        rv.case_to_code = {
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MATH): codegen.SLTBENCH_TEST_MATH_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MATH): codegen.SLTBENCH_TEST_MATH_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MATH): codegen.SLTBENCH_TEST_MATH_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MATH): codegen.SLTBENCH_TEST_MATH_LARGE,
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MALLOC): codegen.SLTBENCH_TEST_MALLOC_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MALLOC): codegen.SLTBENCH_TEST_MALLOC_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MALLOC): codegen.SLTBENCH_TEST_MALLOC_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MALLOC): codegen.SLTBENCH_TEST_MALLOC_LARGE,
        }
        return rv
    if args.backend == backends_common.GOOGLEBENCH:
        rv = backends_common.BackendGooglebench(install_path=args.backend_install_path)
        rv.case_to_code = {
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MATH): codegen.GOOGLEBENCH_TEST_MATH_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MATH): codegen.GOOGLEBENCH_TEST_MATH_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MATH): codegen.GOOGLEBENCH_TEST_MATH_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MATH): codegen.GOOGLEBENCH_TEST_MATH_LARGE,
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MALLOC): codegen.GOOGLEBENCH_TEST_MALLOC_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MALLOC): codegen.GOOGLEBENCH_TEST_MALLOC_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MALLOC): codegen.GOOGLEBENCH_TEST_MALLOC_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MALLOC): codegen.GOOGLEBENCH_TEST_MALLOC_LARGE,
        }
        return rv
    if args.backend == backends_common.NONIUS:
        rv = backends_common.BackendNonius(install_path=args.backend_install_path)
        rv.case_to_code = {
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MATH): codegen.NONIUS_TEST_MATH_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MATH): codegen.NONIUS_TEST_MATH_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MATH): codegen.NONIUS_TEST_MATH_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MATH): codegen.NONIUS_TEST_MATH_LARGE,
            _casekey(dataset.SIZE_MICRO , dataset.KIND_MALLOC): codegen.NONIUS_TEST_MALLOC_MICRO,
            _casekey(dataset.SIZE_SMALL , dataset.KIND_MALLOC): codegen.NONIUS_TEST_MALLOC_SMALL,
            _casekey(dataset.SIZE_MEDIUM, dataset.KIND_MALLOC): codegen.NONIUS_TEST_MALLOC_MEDIUM,
            _casekey(dataset.SIZE_LARGE , dataset.KIND_MALLOC): codegen.NONIUS_TEST_MALLOC_LARGE,
        }
        return rv
    raise RuntimeError('Unrecognized backend: {}'.format(args.backend))
