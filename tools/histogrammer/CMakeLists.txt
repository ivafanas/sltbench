add_executable(sltbench_histogrammer
	HistogramBuilder.cpp
	HistogramBuilder.h
	main.cpp)

add_dependencies(sltbench_histogrammer sltbench)

set_target_properties(sltbench_histogrammer PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_link_libraries(sltbench_histogrammer PRIVATE sltbench)
