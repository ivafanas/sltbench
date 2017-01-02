
# sizes
SIZE_MICRO = 'micro'
SIZE_SMALL = 'small'
SIZE_MEDIUM = 'medium'
SIZE_LARGE = 'large'
SIZES_ALL = [
    SIZE_MICRO,
    SIZE_SMALL,
    SIZE_MEDIUM,
    SIZE_LARGE
]
SIZES_INPUT_ALL = ['all'] + SIZES_ALL

# kinds
KIND_MATH = 'math'
KIND_MALLOC = 'malloc'
KINDS_ALL = [KIND_MATH, KIND_MALLOC]
KINDS_INPUT_ALL = ['all'] + KINDS_ALL


def _create_sizes(args):
    if args.sizes == 'all':
        return SIZES_ALL
    return [args.sizes]


def _create_kinds(args):
    if args.kinds == 'all':
        return KINDS_ALL
    return [args.kinds]


def create(args):
    from collections import namedtuple
    return namedtuple('Dataset', 'sizes,kinds')(
        sizes=_create_sizes(args),
        kinds=_create_kinds(args))
