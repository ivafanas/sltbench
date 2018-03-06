
set(SLTBENCH_PUBLIC_HEADERS_PATH "${PROJECT_SOURCE_DIR}/include")
set(SLTBENCH_LIB_SOURCES
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/Bench.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/BenchApi.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/BenchmarksContainer.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_AG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_F.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_F_AG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_F_LAG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_FB.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_FB_AG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_FB_LAG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Benchmark_LAG.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Descriptor.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/DoNotOptimize.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Env.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IConfig.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IFilter.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IMeasureAlgo.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IReporter.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IRunner.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Optional.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/RunWarnings.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/SingleMeasureAlgo.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/StaticAssertsUtil.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/StopGenerationException.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Verdict.h"
	BenchApi.cpp
	Config.cpp
	Config.h
	ConsoleReporter.cpp
	ConsoleReporter.h
	CsvReporter.cpp
	CsvReporter.h
	DoNotOptimize.cpp
	Env.cpp
	Filters.cpp
	Filters.h
	JsonReporter.cpp
	JsonReporter.h
	MAResultsContainer.cpp
	MAResultsContainer.h
	MeasureAlgo.cpp
	MeasureAlgo.h
	ProcedureBenchmark.cpp
	ProcedureBenchmark.h
	ProgramOptions.cpp
	ProgramOptions.h
	Runner.cpp
	Runner.h
	SingleMeasureAlgo.cpp
	SysInfo.cpp
	SysInfo.h
	Verdict.cpp
	WarningsMessages.h
	WarningsMessages.cpp)

# split source files into nested source groups by subfolder
foreach(source IN LISTS SLTBENCH_LIB_SOURCES)
    get_filename_component(source_path "${source}" PATH)
	string(REPLACE "${SLTBENCH_PUBLIC_HEADERS_PATH}/" "\\" source_rel_path "${source_path}")
    string(REPLACE "/" "\\" source_path_msvc "${source_rel_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# lib
add_library(sltbench ${SLTBENCH_LIB_SOURCES})
add_library(slt::sltbench ALIAS sltbench)

set_target_properties(sltbench PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_include_directories(sltbench PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
    TARGETS sltbench
    EXPORT  ${CMAKE_PROJECT_NAME}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/sltbench" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
