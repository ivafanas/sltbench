
# suites
SIMPLE = 'simple'
ARGS = 'args'
GENERATOR = 'generator'
LAZY_GENERATOR = 'lazy_generator'
FIXTURE = 'fixture'
FIXTURE_ARGS = 'fixture_args'
FIXTURE_GENERATOR = 'fixture_generator'
FIXTURE_LAZY_GENERATOR = 'fixture_lazy_generator'
FIXTURE_BUILDER = 'fixture_builder'
FIXTURE_BUILDER_ARGS = 'fixture_builder_args'
FIXTURE_BUILDER_GENERATOR = 'fixture_builder_generator'
FIXTURE_BUILDER_LAZY_GENERATOR = 'fixture_builder_lazy_generator'


# dataset used to compare different backends
# sltbench supports: all
# googlebench supports: SIMPLE && FIXTURE
# nonius supports: SIMPLE
COMPARABLE = [
    SIMPLE,
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
    FIXTURE_LAZY_GENERATOR,
    FIXTURE_BUILDER,
    FIXTURE_BUILDER_ARGS,
    FIXTURE_BUILDER_GENERATOR,
    FIXTURE_BUILDER_LAZY_GENERATOR
]

# available input
ALL_INPUT = ['comparable', 'all'] + ALL


def create(args):
    if args.dataset == 'comparable':
        return COMPARABLE
    if args.dataset == 'all':
        return ALL
    return [args.dataset]
