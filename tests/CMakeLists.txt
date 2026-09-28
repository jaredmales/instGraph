add_library(instGraphTestMain OBJECT testMain.cpp)
target_include_directories(instGraphTestMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(instGraphXML_test
    $<TARGET_OBJECTS:instGraphTestMain>
    src/instGraphXML_test.cpp
)
target_link_libraries(instGraphXML_test PRIVATE instGraph-shared)
add_test(NAME instGraphXML_test COMMAND instGraphXML_test)
