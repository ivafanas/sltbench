

def gen_cmakelists(sources, backend):
    return '''
        cmake_minimum_required (VERSION 2.8.0)

        project (runner)

        # setup env
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} -std=c++11")

        # sources
        set(BC_SOURCES {sources_list})

        # include backend
        include_directories({backend_install_path}/include)
        link_directories({backend_install_path}/lib)

        # build exe-file
        add_executable(runner ${{BC_SOURCES}})

        # link with sltbench
        target_link_libraries(runner LINK_PUBLIC {backend_static_lib} {required_static_libs})
        '''.format(
            sources_list=' '.join(sources),
            backend_install_path=backend.install_path,
            backend_static_lib=backend.static_lib_name,
            required_static_libs=' '.join(backend.required_static_libs))
