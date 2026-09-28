if(NOT DEFINED COVERAGE_REPORT_DIR OR COVERAGE_REPORT_DIR STREQUAL "")
    message(FATAL_ERROR "COVERAGE_REPORT_DIR is required")
endif()

set(_coverage_index "${COVERAGE_REPORT_DIR}/index.html")
if(EXISTS "${_coverage_index}")
    return()
endif()

file(MAKE_DIRECTORY "${COVERAGE_REPORT_DIR}")
file(WRITE "${_coverage_index}" "<!doctype html>
<html lang=\"en\">
<head>
  <meta charset=\"utf-8\">
  <title>instGraph coverage report</title>
  <link href=\"../doxygen.css\" rel=\"stylesheet\" type=\"text/css\">
</head>
<body>
  <h1>Coverage report has not been generated.</h1>
  <p>Run <code>cmake --build _build --target coverage</code> to generate it.</p>
</body>
</html>
")
