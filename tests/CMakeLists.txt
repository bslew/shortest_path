project (ShortestPath)

set (LIBRARIES 
	minpath	
)

add_executable(make_graph make_graph.cpp)
add_executable(make_FieldGraph make_FieldGraph.cpp)

target_link_libraries(make_graph ${LIBRARIES})
target_link_libraries(make_FieldGraph ${LIBRARIES})

add_subdirectory(unit)
