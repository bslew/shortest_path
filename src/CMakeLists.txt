project (ShortestPath)

add_library(minpath SHARED
	Node.cpp
	Path.cpp
	Graph.cpp
	Obstacles.cpp
	Logger.cpp
)

set (LIBRARIES 
	minpath	
	boost_program_options
	boost_filesystem
	boost_system
	fmt
)

add_executable(shortest_path shortest_path.cpp)
#add_executable(test test.cpp)

target_link_libraries(minpath fmt)
target_link_libraries(shortest_path ${LIBRARIES})
