add_executable(sltbench_unittests
	Benchmark_AG_Test.cpp
	Benchmark_F_Test.cpp
	Benchmark_F_AG_Test.cpp
	Benchmark_F_LAG_Test.cpp
	Benchmark_FB_Test.cpp
	Benchmark_FB_AG_Test.cpp
	Benchmark_FB_LAG_Test.cpp
	Benchmark_LAG_Test.cpp
	FiltersTest.cpp
	main.cpp
	MAResultsContainerTest.cpp
	MeasureAlgoTest.cpp
	OptionalTest.cpp
	ProcedureBenchmarkTest.cpp
	ProgramOptionsTest.cpp
	SingleMeasureAlgoTest.cpp)

add_dependencies(sltbench_unittests sltbench)

set_target_properties(sltbench_unittests PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_include_directories(sltbench_unittests PRIVATE
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/tests/gtest/googletest"
	"${PROJECT_SOURCE_DIR}/tests/gtest/googletest/include")

find_package(Threads REQUIRED)
target_link_libraries(sltbench_unittests PRIVATE sltbench ${CMAKE_THREAD_LIBS_INIT})
