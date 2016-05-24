# Experimental 2D Engine
[![Build Status](https://travis-ci.org/AnthonySuper/Experimental-2D-Engine.svg?branch=master)](https://travis-ci.org/AnthonySuper/Experimental-2D-Engine)

An experimental 2D Game Engine with Ruby as its scripting language.

# Build
You must have MRuby, SDL2, SDL2_ttf, and SDL2_img installed to build.

Building is simple.
Simply create a `build` directory in the root directory of the project, `cd` into it, and type `cmake ..`.
This will generate a `Makefile` you can execute with `make`.

CMake is able to generate many other project files as well.
Check [here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html) for more information.
