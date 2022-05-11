cmake_minimum_required (VERSION 2.6)

project (shortest_path)

find_package(Boost REQUIRED)
find_package(spdlog 1.5 CONFIG REQUIRED)

set (MINPATH_HEADERS 
	path.h
)
include_directories("include")

add_subdirectory(src)
add_subdirectory(tests)
