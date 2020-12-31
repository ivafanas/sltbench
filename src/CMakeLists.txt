
set(SLTBENCH_PUBLIC_HEADERS_PATH "${PROJECT_SOURCE_DIR}/include")
set(SLTBENCH_LIB_SOURCES
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/Bench.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/BenchCore.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/BenchApi.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/BenchCoreApi.h"
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
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IBenchmark.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/IReporter.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Optional.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/RunWarnings.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/StaticAssertsUtil.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/StopGenerationException.h"
	"${SLTBENCH_PUBLIC_HEADERS_PATH}/sltbench/impl/Verdict.h"
	Algorithms.h
	BenchApi.cpp
	BenchmarksContainer.cpp
	BenchmarksContainerPrivate.h
	Config.cpp
	Config.h
	DoNotOptimize.cpp
	Env.cpp
	EnvImpl.h
	Filters.cpp
	Filters.h
	IBenchmark.cpp
	MAResultsContainer.cpp
	MAResultsContainer.h
	MeasureAlgo.cpp
	MeasureAlgo.h
	ProcedureBenchmark.cpp
	ProcedureBenchmark.h
	ProgramOptions.cpp
	ProgramOptions.h
	Reporters.cpp
	Reporters.h
	SingleMeasureAlgo.cpp
	SingleMeasureAlgo.h
	SysInfo.cpp
	SysInfo.h
	Verdict.cpp)

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

if (MSVC)
	target_compile_options (sltbench PRIVATE /W4 /WX /sdl)
else ()
	target_compile_options (sltbench PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif ()

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
