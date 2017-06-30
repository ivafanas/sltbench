

def gen_cmakelists(sources, backend):
    '''
    Generates CMakeLists.txt content for backend
    and performance tests sources set
    '''

    # link commands against benchmark library.
    # makes sense for non header-only frameworks only.
    link_to_benchmark_commands = ''
    if not backend.is_header_only:
        link_to_benchmark_commands = '''
        find_library(BENCHMARK_LIB {benchmark_lib} HINTS {backend_install_path}/lib NO_DEFAULT_PATH)
        target_link_libraries(runner PRIVATE ${{BENCHMARK_LIB}})
        '''.format(
            benchmark_lib=backend.static_lib_name,
            backend_install_path=backend.install_path)

    return '''
        cmake_minimum_required (VERSION 3.5.0)

        project (runner)

        add_executable(runner {sources_list})

        {link_to_benchmark_commands}

        target_link_libraries(runner LINK_PUBLIC {link_libraries})

        target_include_directories(runner PRIVATE {backend_install_path}/include)

        set_target_properties(runner PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)
        '''.format(
            sources_list=' '.join(sources),
            backend_install_path=backend.install_path,
            link_to_benchmark_commands=link_to_benchmark_commands,
            link_libraries=' '.join(backend.required_static_libs))

