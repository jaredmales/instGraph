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

## Demonstration

See [demo 1](doc/demo1.md)

## Future Plans

- [ ] Instrument state: produce a record of current state, e.g. as `toml` or equivalently `JSON`
- [ ] Qt widgets for display
- [ ] User-defined beam sub-states, e.g. bandpass being passed through a filter wheel
