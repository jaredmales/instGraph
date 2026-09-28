cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project("instGraph")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++20")


#######################################################################
#
#                            Source
#
#######################################################################
add_subdirectory(src)


option(INSTGRAPH_BUILD_TESTS "Build instGraph Catch2 tests" OFF)
if(INSTGRAPH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
