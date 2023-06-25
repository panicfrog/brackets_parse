cmake_minimum_required(VERSION 3.24)
project(brackets_parse)

set(SRC main.cpp)
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-mavx2" COMPILER_SUPPORTS_AVX2)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    set(COMPILER_SUPPORTS_NEON TRUE)
endif()
if(COMPILER_SUPPORTS_AVX2)
    message(STATUS "AVX2 support detected; compiling with AVX2 instructions")
    add_definitions("-mavx2")
    list(APPEND SRC find_brackets_positions_avx2.cpp)
elseif(COMPILER_SUPPORTS_NEON)
    message(STATUS "NEON support detected; compiling with NEON instructions")
    list(APPEND SRC find_brackets_positions_neon.cpp)
else()
    message(STATUS "No SIMD support detected; compiling with scalar instructions")
    list(APPEND SRC find_brackets_positions_scalar.cpp)
endif()


set(CMAKE_CXX_STANDARD 17)

add_executable(brackets_parse ${SRC})
