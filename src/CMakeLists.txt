

# list of source files
set(libsrc instGraph.cpp instGraphXML.cpp instNode.cpp instIOPut.cpp instBeam.cpp)

# this is the "object library" target: compiles the sources only once
add_library(objlib OBJECT ${libsrc})

# shared libraries need PIC
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(instGraph-shared SHARED $<TARGET_OBJECTS:objlib>)
add_library(instGraph-static STATIC $<TARGET_OBJECTS:objlib>)

#This makes it so we have libinstGraph.so and libinstGraph.a, without the -shared/-static
SET_TARGET_PROPERTIES(instGraph-shared PROPERTIES OUTPUT_NAME instGraph CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(instGraph-static PROPERTIES OUTPUT_NAME instGraph CLEAN_DIRECT_OUTPUT 1)

#######################################################################
#
#                            Installation
#
#######################################################################

install (TARGETS instGraph-shared DESTINATION lib)
install (TARGETS instGraph-static DESTINATION lib)
install (FILES instGraph.hpp instGraphXML.hpp instNode.hpp instIOPut.hpp instBeam.hpp basicTypes.hpp DESTINATION include/instGraph)

