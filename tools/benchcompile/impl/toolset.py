
from collections import namedtuple

GCC = 'gcc'
CLANG = 'clang'
ALL = [GCC, CLANG]


_Toolset = namedtuple('Toolset', 'c_compiler,cxx_compiler')


def create(args):
    if args.toolset == GCC:
        return _Toolset(c_compiler='gcc', cxx_compiler='g++')
    if args.toolset == CLANG:
        return _Toolset(c_compiler='clang', cxx_compiler='clang++')
    raise RuntimeError('Unsupported toolset: {}'.format(args.toolset))
