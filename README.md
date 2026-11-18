# game-of-life
A C++ implementation of Conway's Game of Life

[![Build Status](https://img.shields.io/travis/MykhailoHalchenko/game-of-life.svg?branch=main)](https://travis-ci.org/MykhailoHalchenko/game-of-life)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Conway's Game of Life is a cellular automaton devised by mathematician John Horton Conway. This project provides a C++ implementation for simulating and visualizing the Game of Life. It's designed for users interested in exploring cellular automata and practicing C++ development.

## Features

*   Grid-based simulation of cellular states.
*   Rules for cell birth, survival, and death.
*   Optional visualization of the grid.
*   Customizable grid dimensions.

## Installation

To build and run the project, you'll need a C++ compiler (e.g., g++) and CMake.

```bash
git clone https://github.com/MykhailoHalchenko/game-of-life.git
cd game-of-life
mkdir build
cd build
cmake ..
make
```

## Quick start / Usage

After building, you can run the executable from the `build` directory.

```bash
./game-of-life <rows> <cols> <generations> [initial_pattern_file]
```

For example, to run a simulation with a 20x20 grid for 100 generations:

```bash
./game-of-life 20 20 100
```

## Contributing

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute to this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
