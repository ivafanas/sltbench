
# suites
SIMPLE = 'simple'
ARGS = 'args'
GENERATOR = 'generator'
LAZY_GENERATOR = 'lazy_generator'
FIXTURE = 'fixture'
FIXTURE_ARGS = 'fixture_args'
FIXTURE_GENERATOR = 'fixture_generator'
FIXTURE_LAZY_GENERATOR = 'fixture_lazy_generator'

# dataset used to compare different backends
COMPARABLE = [
    SIMPLE,
    FIXTURE,
]

# all suites
ALL = [
    SIMPLE,
    ARGS,
    GENERATOR,
    LAZY_GENERATOR,
    FIXTURE,
    FIXTURE_ARGS,
    FIXTURE_GENERATOR,
    FIXTURE_LAZY_GENERATOR
]

# available input
ALL_INPUT = ['comparable', 'all'] + ALL


def create(args):
    if args.dataset == 'comparable':
        return COMPARABLE
    if args.dataset == 'all':
        return ALL
    return [args.dataset]
