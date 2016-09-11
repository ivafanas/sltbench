cmake_minimum_required (VERSION 2.8.0)

project (sltbench)

# setup env
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# options
option(BUILD_TESTS "BUILD_TESTS" OFF)

# sources
set(SLTBENCH_LIB_SOURCES
	src/BenchApi.cpp
	src/Config.cpp
	src/Config.h
	src/ConsoleReporter.cpp
	src/ConsoleReporter.h
	src/CsvReporter.cpp
	src/CsvReporter.h
	src/Env.cpp
	src/Filters.cpp
	src/Filters.h
	src/JsonReporter.cpp
	src/JsonReporter.h
	src/MeasureAlgo.cpp
	src/MeasureAlgo.h
	src/ProcedureBenchmark.cpp
	src/ProcedureBenchmark.h
	src/ProgramOptions.cpp
	src/ProgramOptions.h
	src/Runner.cpp
	src/Runner.h)

# public headers
set(SLTBENCH_LIB_PUBLIC_HEADERS
	include/sltbench/Bench.h)

# public implementation headers
set(SLTBENCH_LIB_PUBLIC_IMPL_HEADERS
	include/sltbench/impl/BenchApi.h
	include/sltbench/impl/BenchmarksContainer.h
	include/sltbench/impl/BenchmarkWithArgGenerator.h
	include/sltbench/impl/BenchmarkWithFixture.h
	include/sltbench/impl/BenchmarkWithFixtureAndArgGenerator.h
	include/sltbench/impl/BenchmarkWithFixtureAndLazyArgGenerator.h
	include/sltbench/impl/BenchmarkWithLazyArgGenerator.h
	include/sltbench/impl/Descriptor.h
	include/sltbench/impl/Env.h
	include/sltbench/impl/IConfig.h
	include/sltbench/impl/IFilter.h
	include/sltbench/impl/IMeasureAlgo.h
	include/sltbench/impl/IReporter.h
	include/sltbench/impl/IRunner.h
	include/sltbench/impl/StaticAssertsUtil.h
	include/sltbench/impl/StopGenerationException.h)

# examples sources
set(SLTBENCH_EXAMPLES_SOURCES
	examples/custom_reporter.cpp
	examples/demo.cpp
	examples/function_with_args.cpp
	examples/function_with_args_generator.cpp
	examples/function_with_exception.cpp
	examples/function_with_fixture.cpp
	examples/function_with_fixture_and_args.cpp
	examples/function_with_fixture_and_args_generator.cpp
	examples/function_with_fixture_and_lazy_args_generator.cpp
	examples/function_with_lazy_args_generator.cpp
	examples/main.cpp
	examples/simple_function.cpp)

# autolearning sources
set(SLTBENCH_AUTOLEARNING_SOURCES
	tools/autolearning/main.cpp
	tools/autolearning/Samples.cpp)

# histogrammer sources
set(SLTBENCH_HISTOGRAMMER_SOURCES
	tools/histogrammer/HistogramBuilder.cpp
	tools/histogrammer/HistogramBuilder.h
	tools/histogrammer/main.cpp)

# tests sources
set(SLTBENCH_TESTS_SOURCES
	tests/BenchmarkWithArgGeneratorTest.cpp
	tests/BenchmarkWithFixtureAndArgGeneratorTest.cpp
	tests/BenchmarkWithFixtureAndLazyArgGeneratorTest.cpp
	tests/BenchmarkWithLazyArgGeneratorTest.cpp
	tests/FiltersTest.cpp
	tests/main.cpp
	tests/MeasureAlgoTest.cpp
	tests/ProcedureBenchmarkTest.cpp
	tests/ProgramOptionsTest.cpp
	tests/RunnerTest.cpp)

# split source files into nested source groups by subfolder
foreach(source IN LISTS SLTBENCH_LIB_SOURCES SLTBENCH_LIB_PUBLIC_HEADERS SLTBENCH_LIB_PUBLIC_IMPL_HEADERS SLTBENCH_TESTS_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

# lib
add_library (sltbench_static
	${SLTBENCH_LIB_SOURCES}
	${SLTBENCH_LIB_PUBLIC_HEADERS}
	${SLTBENCH_LIB_PUBLIC_IMPL_HEADERS})

# examples
add_executable(sltbench_examples ${SLTBENCH_EXAMPLES_SOURCES})
target_link_libraries(sltbench_examples LINK_PUBLIC sltbench_static)

# autolearning tool
add_executable(sltbench_autolearning ${SLTBENCH_AUTOLEARNING_SOURCES})
target_link_libraries(sltbench_autolearning LINK_PUBLIC sltbench_static)

# histogrammer tool
add_executable(sltbench_histogrammer ${SLTBENCH_HISTOGRAMMER_SOURCES})
target_link_libraries(sltbench_histogrammer LINK_PUBLIC sltbench_static)

# install static lib
install(
	TARGETS sltbench_static
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)
install(
	FILES ${SLTBENCH_LIB_PUBLIC_HEADERS}
	DESTINATION include/sltbench
)
install(
	FILES ${SLTBENCH_LIB_PUBLIC_IMPL_HEADERS}
	DESTINATION include/sltbench/impl
)

# unit tests
if(BUILD_TESTS)
	include_directories (${CMAKE_CURRENT_SOURCE_DIR}/tests/gtest/googletest/include)
	include_directories (${CMAKE_CURRENT_SOURCE_DIR}/tests/gtest/googletest)

	find_package (Threads)
	add_executable(sltbench_unittests ${SLTBENCH_TESTS_SOURCES})
	target_link_libraries(sltbench_unittests LINK_PUBLIC sltbench_static ${CMAKE_THREAD_LIBS_INIT})
endif()
