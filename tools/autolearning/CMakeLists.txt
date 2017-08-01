add_executable(sltbench_autolearning
	main.cpp
	Samples.cpp)

add_dependencies(sltbench_autolearning sltbench)

set_target_properties(sltbench_autolearning PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_link_libraries(sltbench_autolearning PRIVATE sltbench)

target_include_directories(sltbench_autolearning PRIVATE "${PROJECT_SOURCE_DIR}")
