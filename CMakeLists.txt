cmake_minimum_required (VERSION 3.0)

project (ShortestPath)

find_package(Boost REQUIRED)
find_package(spdlog 1.5 CONFIG REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_compile_options(-Wall -Werror)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

include_directories("include")
add_subdirectory(src)
add_subdirectory(tests)
