cmake_minimum_required (VERSION 3.0)

project (ShortestPath)

find_package(Boost REQUIRED)
find_package(spdlog 1.5 CONFIG REQUIRED)
# Add GoogleTest
find_package(GTest REQUIRED)

set (MINPATH_HEADERS 
	path.h
)
include_directories("include")

add_subdirectory(src)
add_subdirectory(tests)
