# C++ Gameboy Emulator [![Build Status](https://github.com/arthurgiroux/gbemulator/actions/workflows/ci.yml/badge.svg)](https://github.com/arthurgiroux/gbemulator/actions/workflows/ci.yml/badge.svg)

This is an educative implementation of a Gameboy Emulator made in C++.

## Features

Here's what's implemented so far:

* Memory abstraction
* All CPU instructions
* PPU abstraction
* GUI
* Sprites
* Inputs
* Timer
* Serial

Future work:

* Memory Bus Controller
* Sound

## Tests

The emulator is tested using Blargg's tests and various automated test cases for all components.
You can find the test report in the Github actions.

Blaarg's test coverage:

| Category            | Test Name               | Pass/Fail           |
|---------------------|-------------------------|---------------------|
| CPU Instructions    | 01 - Special            | :white_check_mark:	 |
| CPU Instructions    | 02 - Interrupts         | :white_check_mark:	 |
| CPU Instructions    | 03 - Op SP, HL          | :white_check_mark:	 |
| CPU Instructions    | 04 - Op r,imm           | :white_check_mark:	 |
| CPU Instructions    | 05 - Op rp              | :white_check_mark:	 |
| CPU Instructions    | 06 - LD r,r             | :white_check_mark:	 |
| CPU Instructions    | 07 - JR,JP,Call,RET,RST | :white_check_mark:	 |
| CPU Instructions    | 08 - Misc Instrs        | :white_check_mark:	 |
| CPU Instructions    | 09 - Op r,r             | :white_check_mark:	 |
| CPU Instructions    | 10 - Bits Ops           | :white_check_mark:	 |
| CPU Instructions    | 11 - Op a,(hl)          | :white_check_mark:	 |
| CPU Instructions    | 07 - JR,JP,Call,RET,RST | :white_check_mark:	 |
| Instructions Timing | Instr Timing            | :white_check_mark:	 |

## Demo

![Demo](demo_image/emulator_demo.gif)

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