project(ShortestPath)

include_directories(${GTEST_INCLUDE_DIRS})

set (LIBRARIES 
	minpath
    pthread
    gtest
)

# Add the test source files
file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
# Create a test executable for each test source file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Link the test executable with GoogleTest and the main project
    target_link_libraries(${TEST_NAME} ${LIBRARIES})

    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

