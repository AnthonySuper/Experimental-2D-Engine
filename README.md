# Experimental 2D Engine
[![Build Status](https://travis-ci.org/AnthonySuper/Experimental-2D-Engine.svg?branch=master)](https://travis-ci.org/AnthonySuper/Experimental-2D-Engine)

An experimental 2D game engine, using ruby as its scripting language.

# Why are you making this?

Please see [GOALS.md](./GOALS.md) for more information.

# How to Build
You must have `SDL2`, `SDL2_ttf`, and `SDL2_img` installed to build.

Thankfully, our build process is rather simple.
First, `cd` into the `mruby` directory, and use `make`.
This will make the shared `mruby` library, needed to build the rest of the engine.
Next, return to the root directory of the repository.
Make a new directory (we recommend calling it `build`) and `cd` into it, then type `cmake ..`
This will generate a `Makefile` you can execute with `make`.

CMake is able to generate many other project files as well.
Check [here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html) for more information.
