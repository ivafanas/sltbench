from collections import namedtuple


MAKE = 'make'
NINJA = 'ninja'
ALL = [MAKE, NINJA]


_BuildSystem = namedtuple('BuildSystem', 'cmake_generator,build_command')


def create(args):
    bs = args.build_system

    if bs == MAKE:
        return _BuildSystem(cmake_generator='Unix Makefiles',
                            build_command='make -j 1')

    if bs == NINJA:
        return _BuildSystem(cmake_generator='Ninja',
                            build_command='ninja -j 1')

    raise RuntimeError('Unsupported build system: {}'.format(bs))

