cmake_minimum_required (VERSION 3.5.0)

project (sltbench)


# options
option(SLT_BUILD_TESTS "SLT_BUILD_TESTS" OFF)
option(SLT_BUILD_EXAMPLES "SLT_BUILD_EXAMPLES" OFF)
option(SLT_BUILD_HISTOGRAMMER "SLT_BUILD_HISTOGRAMMER" OFF)
option(SLT_BUILD_AUTOLEARNING "SLT_BUILD_AUTOLEARNING" OFF)

add_subdirectory(src)
install(FILES sltbench-config.cmake DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake")

if (SLT_BUILD_TESTS)
	add_subdirectory(tests)
endif()

if (SLT_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if (SLT_BUILD_AUTOLEARNING)
	add_subdirectory(tools/autolearning)
endif()

if (SLT_BUILD_HISTOGRAMMER)
	add_subdirectory(tools/histogrammer)
endif()
