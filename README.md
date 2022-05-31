# C++ Gameboy Emulator [![Build Status](https://app.travis-ci.com/arthurgiroux/gbemulator.svg?branch=master)](https://app.travis-ci.com/arthurgiroux/gbemulator)

This is an educative implementation of a Gameboy Emulator made in C++.

## Features

Here's what's implemented so far:
* Memory abstraction
* All CPU instructions
* GPU abstraction
* Very simple GUI
* Graphics (WIP)

Future work:
* Sound
* Inputs
* Advanced GUI

## Compilation

The project can be compiled using CMake, it will automatically retrieve its dependencies.

```
mkdir -p build && cd build/
cmake ..
cmake --build .
```

## Dependencies

This project relies on ImGUI and SDL for the display and GoogleTest for the testing suite.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)