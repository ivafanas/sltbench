

def gen_cmakelists(sources, backend):
    # build required link libraries:
    # append required static libs and the self benchmark library
    # if it is not header-only (then backend.static_lib_name is None)
    link_libraries = []
    link_libraries.extend(backend.required_static_libs)
    if not backend.is_header_only:
        link_libraries.append(backend.static_lib_name)

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

        add_executable(runner ${{BC_SOURCES}})
        target_link_libraries(runner LINK_PUBLIC {link_libraries})
        '''.format(
            sources_list=' '.join(sources),
            backend_install_path=backend.install_path,
            link_libraries=' '.join(link_libraries))
