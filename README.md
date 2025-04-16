# Mosaic Game: Extended Version

## Project Overview

Mosaic is a logical puzzle game for a single player where participants fill a grid with black or white squares according to numerical clues. This extended version introduces features such as rectangular grids, toroidal topology, variable neighborhood constraints, and a move history with undo and redo capabilities.

## Key Features

- Rectangular grids with a maximum size of 5x5.
- Toroidal grid option for continuous edges.
- Four neighborhood constraint modes: FULL, ORTHO, FULL_EXCLUDE, ORTHO_EXCLUDE.
- Move history tracking with undo and redo functionality.

## Project Structure

- `game.h`/`game.c`: Core game logic and structures.
- `game_aux.h`/`game_aux.c`: Auxiliary functions for game management.
- `game_ext.h`/`game_ext.c`: Extended features for the new version of the game.
- `game_struct.h`: Shared definitions of the game structure.
- `game_text.c`: Text-based interface for playing the game.
- `queue.h`/`queue.c`: Double-ended queue implementation for move history.
- `CMakeLists.txt`: CMake configuration file for building the project.

## Build Instructions

Use CMake and Make to build the game:
```sh
mkdir build
cd build
cmake ..
make
```

## Playing the Game

Run the executable from the build directory to play in text mode:
```sh
./game_text
```

## Basic Commands

- h - Display help.
- r - Restart the game.
- q - Quit the game.
- w, b, e - Play a move (white, black, or empty) followed by grid coordinates.

## Extended Commands

- undo - Undo the last move.
- redo - Redo the last undone move.

## Tests

The project includes unit tests to validate game functionality. Run the tests with:
```sh
make test
```

## Live Demo

Try the web-based demo at: [https://anas-el-mouden.emi.u-bordeaux.fr/make-game-web/demo.html](https://anas-el-mouden.emi.u-bordeaux.fr/make-game-web/demo.html)

## Acknowledgements

- Inspired by Simon Tatham's Portable Puzzle Collection.
- University of Bordeaux for guidance and project specifications.