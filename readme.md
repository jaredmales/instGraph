# instGraph

A way to represent an astronomical instrument's logical state.

## Description

to-do

## Building 

Building the instGraph library follows the usual cmake process:

```bash
mkdir _build
cd _build
cmake ..
make
sudo make install
```

Note that you do not need to build the library to run the demo.

## XML output and tests

`instGraphXML` saves automatically after graph mutations by default. Applications that publish a complete snapshot after a group of mutations can call `autoSave(false)` and then `serializeXML(xml, error)` to obtain the current XML without writing a file. Automatic saves now throw `std::runtime_error` if the configured output cannot be written.

The unit tests use Catch2 and are enabled through CMake:

```bash
cmake -S . -B _build -DINSTGRAPH_BUILD_TESTS=ON
cmake --build _build -j
ctest --test-dir _build --output-on-failure
```

## Coverage and documentation

GCC, `lcov`, `genhtml`, and Doxygen are needed to generate the HTML coverage report. The `coverage` target configures a separate instrumented build, runs the Catch2 suite through CTest, and embeds the resulting report in the Doxygen HTML:

```bash
cmake -S . -B _build
cmake --build _build --target coverage
# Open _build/doc/html/group__instGraph__coverage.html
```

The report itself is at `_build/doc/html/coverage/index.html`. A `docs` build creates the Doxygen pages with a placeholder until coverage is run:

```bash
cmake --build _build --target docs
```

Set `INSTGRAPH_BUILD_DOCS=ON` to include docs in the default build. `INSTGRAPH_COVERAGE_TEST_TIMEOUT` controls the CTest timeout (default 300 seconds). Clean generated reports with `cmake --build _build --target coverage_clean`; use `docs_clean` to remove the documentation tree. From the repository root, `tests/coverage/make_coverage` configures and builds the report, while `tests/coverage/update_coverage` refreshes an existing `_build` tree.

## Demonstration

See [demo 1](doc/demo1.md)

## Future Plans

- [ ] Instrument state: produce a record of current state, e.g. as `toml` or equivalently `JSON`
- [ ] Qt widgets for display
- [ ] User-defined beam sub-states, e.g. bandpass being passed through a filter wheel
