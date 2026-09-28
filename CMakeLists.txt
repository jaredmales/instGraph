cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project("instGraph")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++20")

option(INSTGRAPH_BUILD_COVERAGE "Enable GCC coverage instrumentation" OFF)
set(INSTGRAPH_COVERAGE_TEST_TIMEOUT 300 CACHE STRING "CTest timeout (seconds) for coverage target")
option(INSTGRAPH_BUILD_DOCS "Build Doxygen docs as part of the default build" OFF)

if(INSTGRAPH_BUILD_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "INSTGRAPH_BUILD_COVERAGE requires GCC/gcov")
    endif()
    # Append -O0 after the project's usual -O2 flag so counters reflect test paths.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

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

#######################################################################
#                       Documentation and coverage
#######################################################################
set(INSTGRAPH_DOC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")
set(INSTGRAPH_DOC_HTML_DIR "${INSTGRAPH_DOC_OUTPUT_DIR}/html")
set(INSTGRAPH_COVERAGE_REPORT_DIR "${INSTGRAPH_DOC_HTML_DIR}/coverage")
set(INSTGRAPH_COVERAGE_BUILD_DIR "${CMAKE_BINARY_DIR}/coverage-build")
set(INSTGRAPH_COVERAGE_BASELINE_INFO "${CMAKE_BINARY_DIR}/coverage_baseline.info")
set(INSTGRAPH_COVERAGE_RUNTIME_INFO "${CMAKE_BINARY_DIR}/coverage_runtime.info")
set(INSTGRAPH_COVERAGE_INFO "${CMAKE_BINARY_DIR}/coverage.info")
set(INSTGRAPH_COVERAGE_FILTERED_INFO "${CMAKE_BINARY_DIR}/coverage_filtered.info")

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    file(READ "${CMAKE_SOURCE_DIR}/doc/instGraph.dox" _instgraph_doxyfile)
    set(_instgraph_doxyfile_source "${_instgraph_doxyfile}")
    string(REPLACE "OUTPUT_DIRECTORY       =\n"
        "OUTPUT_DIRECTORY       = ${INSTGRAPH_DOC_OUTPUT_DIR}\n"
        _instgraph_doxyfile "${_instgraph_doxyfile}")
    if(_instgraph_doxyfile STREQUAL _instgraph_doxyfile_source)
        message(FATAL_ERROR "Could not set Doxygen OUTPUT_DIRECTORY in doc/instGraph.dox")
    endif()
    file(WRITE "${CMAKE_BINARY_DIR}/instGraph.dox" "${_instgraph_doxyfile}")

    if(INSTGRAPH_BUILD_DOCS)
        set(_instgraph_docs_all ALL)
    else()
        set(_instgraph_docs_all)
    endif()
    add_custom_target(docs ${_instgraph_docs_all}
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/instGraph.dox"
        COMMAND "${CMAKE_COMMAND}"
            "-DCOVERAGE_REPORT_DIR=${INSTGRAPH_COVERAGE_REPORT_DIR}"
            -P "${CMAKE_SOURCE_DIR}/cmake/ensure_coverage_placeholder.cmake"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc"
        USES_TERMINAL
        VERBATIM
    )
    add_custom_target(docs_clean
        COMMAND "${CMAKE_COMMAND}" -E rm -rf "${INSTGRAPH_DOC_OUTPUT_DIR}"
        VERBATIM
    )
else()
    message(WARNING "Doxygen not found; docs target unavailable")
endif()

find_program(INSTGRAPH_LCOV_EXECUTABLE lcov)
find_program(INSTGRAPH_GENHTML_EXECUTABLE genhtml)
if(INSTGRAPH_LCOV_EXECUTABLE AND INSTGRAPH_GENHTML_EXECUTABLE)
    add_custom_target(coverage
        COMMAND "${CMAKE_COMMAND}" -S "${CMAKE_SOURCE_DIR}" -B "${INSTGRAPH_COVERAGE_BUILD_DIR}"
            -G "${CMAKE_GENERATOR}"
            "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
            -DCMAKE_BUILD_TYPE=Debug
            -DINSTGRAPH_BUILD_TESTS=ON
            -DINSTGRAPH_BUILD_COVERAGE=ON
            -DINSTGRAPH_BUILD_DOCS=OFF
        COMMAND "${CMAKE_COMMAND}" --build "${INSTGRAPH_COVERAGE_BUILD_DIR}" -j
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --directory "${INSTGRAPH_COVERAGE_BUILD_DIR}" --zerocounters
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --directory "${INSTGRAPH_COVERAGE_BUILD_DIR}" --capture --initial
            --output-file "${INSTGRAPH_COVERAGE_BASELINE_INFO}"
        COMMAND "${CMAKE_CTEST_COMMAND}" --test-dir "${INSTGRAPH_COVERAGE_BUILD_DIR}"
            --output-on-failure --timeout "${INSTGRAPH_COVERAGE_TEST_TIMEOUT}"
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --directory "${INSTGRAPH_COVERAGE_BUILD_DIR}" --capture
            --output-file "${INSTGRAPH_COVERAGE_RUNTIME_INFO}"
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --add-tracefile "${INSTGRAPH_COVERAGE_BASELINE_INFO}"
            --add-tracefile "${INSTGRAPH_COVERAGE_RUNTIME_INFO}"
            --ignore-errors inconsistent,inconsistent
            --output-file "${INSTGRAPH_COVERAGE_INFO}"
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --extract "${INSTGRAPH_COVERAGE_INFO}"
            "${CMAKE_SOURCE_DIR}/src/*"
            --ignore-errors unused,unused
            --ignore-errors inconsistent,inconsistent
            --output-file "${INSTGRAPH_COVERAGE_FILTERED_INFO}"
        COMMAND "${INSTGRAPH_LCOV_EXECUTABLE}" --remove "${INSTGRAPH_COVERAGE_FILTERED_INFO}"
            "${CMAKE_SOURCE_DIR}/src/pugixml/*"
            "${CMAKE_SOURCE_DIR}/src/toml++/*"
            --ignore-errors unused,unused
            --ignore-errors inconsistent,inconsistent
            --output-file "${INSTGRAPH_COVERAGE_FILTERED_INFO}"
        COMMAND "${CMAKE_COMMAND}" -E rm -rf "${INSTGRAPH_COVERAGE_REPORT_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${INSTGRAPH_COVERAGE_REPORT_DIR}"
        COMMAND "${INSTGRAPH_GENHTML_EXECUTABLE}" "${INSTGRAPH_COVERAGE_FILTERED_INFO}"
            --output-directory "${INSTGRAPH_COVERAGE_REPORT_DIR}"
            --title instGraph --hierarchical --merge-aliases --suppress-aliases
            --filter function --demangle-cpp
            --css-file "${CMAKE_SOURCE_DIR}/tests/coverage/gcov.css"
        USES_TERMINAL
        VERBATIM
    )
    if(TARGET docs)
        add_dependencies(coverage docs)
    endif()
else()
    message(WARNING "lcov/genhtml not found; coverage target unavailable")
endif()

add_custom_target(coverage_clean
    COMMAND "${CMAKE_COMMAND}" -E rm -f "${INSTGRAPH_COVERAGE_BASELINE_INFO}"
        "${INSTGRAPH_COVERAGE_RUNTIME_INFO}" "${INSTGRAPH_COVERAGE_INFO}"
        "${INSTGRAPH_COVERAGE_FILTERED_INFO}"
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${INSTGRAPH_COVERAGE_REPORT_DIR}"
        "${INSTGRAPH_COVERAGE_BUILD_DIR}"
    VERBATIM
)
