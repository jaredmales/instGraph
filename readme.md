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

## Demonstration

See [demo 1](doc/demo1.md)

## Future Plans

- [ ] Instrument state: produce a record of current state, e.g. as `toml` or equivalently `JSON`
- [ ] Qt widgets for display
- [ ] User-defined beam sub-states, e.g. bandpass being passed through a filter wheel
