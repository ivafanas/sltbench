option(SLT_ENABLE_ASAN "SLT_ENABLE_ASAN" OFF)
option(SLT_ENABLE_LSAN "SLT_ENABLE_LSAN" OFF)
option(SLT_ENABLE_MSAN "SLT_ENABLE_MSAN" OFF)
option(SLT_ENABLE_TSAN "SLT_ENABLE_TSAN" OFF)
option(SLT_ENABLE_UBSAN "SLT_ENABLE_UBSAN" OFF)

macro(add_sanitizer_compiler_flags flags)
	set(SLTBENCH_SAN_FLAGS "-g -fno-omit-frame-pointer ${flags}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SLTBENCH_SAN_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SLTBENCH_SAN_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SLTBENCH_SAN_FLAGS}")
endmacro()

if(SLT_ENABLE_ASAN)
	add_sanitizer_compiler_flags("-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope")
endif()

if(SLT_ENABLE_LSAN)
	add_sanitizer_compiler_flags("-fsanitize=leak")
endif()

if(SLT_ENABLE_MSAN)
	add_sanitizer_compiler_flags("-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins")
endif()

if(SLT_ENABLE_TSAN)
	add_sanitizer_compiler_flags("-fsanitize=thread")
endif()

if(SLT_ENABLE_UBSAN)
	add_sanitizer_compiler_flags("-fsanitize=undefined")
endif()

