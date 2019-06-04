cmake_minimum_required (VERSION 3.11.4)

project (sltbench)

include(GNUInstallDirs)
set(CMAKE_INSTALL_CONFIGDIR cmake)

# options
option(SLT_BUILD_TESTS "SLT_BUILD_TESTS" OFF)
option(SLT_BUILD_EXAMPLES "SLT_BUILD_EXAMPLES" OFF)
option(SLT_BUILD_HISTOGRAMMER "SLT_BUILD_HISTOGRAMMER" OFF)
option(SLT_BUILD_AUTOLEARNING "SLT_BUILD_AUTOLEARNING" OFF)

add_subdirectory(src)

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

configure_file(${CMAKE_PROJECT_NAME}-config.cmake.in ${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake DESTINATION ${CMAKE_INSTALL_CONFIGDIR})

install(EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_CONFIGDIR} FILE ${CMAKE_PROJECT_NAME}Targets.cmake)
export(EXPORT ${CMAKE_PROJECT_NAME} FILE ${CMAKE_PROJECT_NAME}Targets.cmake)
