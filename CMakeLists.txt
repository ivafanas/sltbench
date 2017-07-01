cmake_minimum_required (VERSION 3.5.0)

project (sltbench)

# options
option(SLT_BUILD_TESTS "SLT_BUILD_TESTS" OFF)
option(SLT_BUILD_EXAMPLES "SLT_BUILD_EXAMPLES" OFF)
option(SLT_BUILD_HISTOGRAMMER "SLT_BUILD_HISTOGRAMMER" OFF)
option(SLT_BUILD_AUTOLEARNING "SLT_BUILD_AUTOLEARNING" OFF)

# sources
set(SLTBENCH_LIB_SOURCES
	src/BenchApi.cpp
	src/Config.cpp
	src/Config.h
	src/ConsoleReporter.cpp
	src/ConsoleReporter.h
	src/CsvReporter.cpp
	src/CsvReporter.h
	src/DoNotOptimize.cpp
	src/Env.cpp
	src/Filters.cpp
	src/Filters.h
	src/JsonReporter.cpp
	src/JsonReporter.h
	src/MAResultsContainer.cpp
	src/MAResultsContainer.h
	src/MeasureAlgo.cpp
	src/MeasureAlgo.h
	src/ProcedureBenchmark.cpp
	src/ProcedureBenchmark.h
	src/ProgramOptions.cpp
	src/ProgramOptions.h
	src/Runner.cpp
	src/Runner.h
	src/SingleMeasureAlgo.cpp
	src/SysInfo.cpp
	src/SysInfo.h
	src/Verdict.cpp
	src/WarningsMessages.h
	src/WarningsMessages.cpp)

# public headers
set(SLTBENCH_LIB_PUBLIC_HEADERS
	include/sltbench/Bench.h)

# public implementation headers
set(SLTBENCH_LIB_PUBLIC_IMPL_HEADERS
	include/sltbench/impl/BenchApi.h
	include/sltbench/impl/BenchmarksContainer.h
	include/sltbench/impl/Benchmark_AG.h
	include/sltbench/impl/Benchmark_F.h
	include/sltbench/impl/Benchmark_F_AG.h
	include/sltbench/impl/Benchmark_F_LAG.h
	include/sltbench/impl/Benchmark_FB.h
	include/sltbench/impl/Benchmark_FB_AG.h
	include/sltbench/impl/Benchmark_FB_LAG.h
	include/sltbench/impl/Benchmark_LAG.h
	include/sltbench/impl/Descriptor.h
	include/sltbench/impl/DoNotOptimize.h
	include/sltbench/impl/Env.h
	include/sltbench/impl/IConfig.h
	include/sltbench/impl/IFilter.h
	include/sltbench/impl/IMeasureAlgo.h
	include/sltbench/impl/IReporter.h
	include/sltbench/impl/IRunner.h
	include/sltbench/impl/Optional.h
	include/sltbench/impl/RunWarnings.h
	include/sltbench/impl/SingleMeasureAlgo.h
	include/sltbench/impl/StaticAssertsUtil.h
	include/sltbench/impl/StopGenerationException.h
	include/sltbench/impl/Verdict.h)

# split source files into nested source groups by subfolder
foreach(source IN LISTS SLTBENCH_LIB_SOURCES SLTBENCH_LIB_PUBLIC_HEADERS SLTBENCH_LIB_PUBLIC_IMPL_HEADERS)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# lib
add_library(sltbench
	${SLTBENCH_LIB_SOURCES}
	${SLTBENCH_LIB_PUBLIC_HEADERS}
	${SLTBENCH_LIB_PUBLIC_IMPL_HEADERS})
add_library(slt::sltbench ALIAS sltbench)

set_target_properties(sltbench PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_include_directories(sltbench
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# install static lib
install(
	TARGETS sltbench
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
if (SLT_BUILD_TESTS)
	add_executable(sltbench_unittests
		tests/Benchmark_AG_Test.cpp
		tests/Benchmark_F_Test.cpp
		tests/Benchmark_F_AG_Test.cpp
		tests/Benchmark_F_LAG_Test.cpp
		tests/Benchmark_FB_Test.cpp
		tests/Benchmark_FB_AG_Test.cpp
		tests/Benchmark_FB_LAG_Test.cpp
		tests/Benchmark_LAG_Test.cpp
		tests/FiltersTest.cpp
		tests/main.cpp
		tests/MAResultsContainerTest.cpp
		tests/MeasureAlgoTest.cpp
		tests/ProcedureBenchmarkTest.cpp
		tests/ProgramOptionsTest.cpp
		tests/SingleMeasureAlgoTest.cpp
		tests/RunnerTest.cpp)
	add_executable(slt::sltbench_unittests ALIAS sltbench_unittests)

	add_dependencies(sltbench_unittests sltbench)

	set_target_properties(sltbench_unittests PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO)

	target_include_directories(sltbench_unittests PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/tests/gtest/googletest
		${CMAKE_CURRENT_SOURCE_DIR}/tests/gtest/googletest/include)

	find_package (Threads)
	target_link_libraries(sltbench_unittests
		LINK_PUBLIC sltbench ${CMAKE_THREAD_LIBS_INIT})
endif()

# examples
if (SLT_BUILD_EXAMPLES)
	add_executable(sltbench_examples
		examples/custom_reporter.cpp
		examples/demo.cpp
		examples/function_with_args.cpp
		examples/function_with_args_generator.cpp
		examples/function_with_exception.cpp
		examples/function_with_fixture.cpp
		examples/function_with_fixture_and_args.cpp
		examples/function_with_fixture_and_args_generator.cpp
		examples/function_with_fixture_and_lazy_args_generator.cpp
		examples/function_with_fixture_builder.cpp
		examples/function_with_fixture_builder_and_args.cpp
		examples/function_with_fixture_builder_and_args_generator.cpp
		examples/function_with_fixture_builder_and_lazy_args_generator.cpp
		examples/function_with_lazy_args_generator.cpp
		examples/main.cpp
		examples/simple_function.cpp)
	add_executable(slt::sltbench_examples ALIAS sltbench_examples)

	add_dependencies(sltbench_examples sltbench)

	set_target_properties(sltbench_examples PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO)

	target_link_libraries(sltbench_examples LINK_PUBLIC sltbench)
endif()

# autolearning tool
if (SLT_BUILD_AUTOLEARNING)
	add_executable(sltbench_autolearning
		tools/autolearning/main.cpp
		tools/autolearning/Samples.cpp)
	add_executable(slt::sltbench_autolearning ALIAS sltbench_autolearning)

	add_dependencies(sltbench_autolearning sltbench)

	set_target_properties(sltbench_autolearning PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO)

	target_link_libraries(sltbench_autolearning LINK_PUBLIC sltbench)

	target_include_directories(sltbench_autolearning PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR})
endif()

# histogrammer tool
if (SLT_BUILD_HISTOGRAMMER)
	add_executable(sltbench_histogrammer
		tools/histogrammer/HistogramBuilder.cpp
		tools/histogrammer/HistogramBuilder.h
		tools/histogrammer/main.cpp)
	add_executable(slt::sltbench_histogrammer ALIAS sltbench_histogrammer)

	add_dependencies(sltbench_histogrammer sltbench)

	set_target_properties(sltbench_histogrammer PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO)

	target_link_libraries(sltbench_histogrammer LINK_PUBLIC sltbench)
endif()
