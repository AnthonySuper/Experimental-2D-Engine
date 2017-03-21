#!/bin/bash
set -ex

# Compile mruby first.
(cd mruby && make -j2)

# Override CXX if specified by the CI environment.
if [ -n "$COMPILER" ]; then
  export CXX="$COMPILER"
fi

# Generate our build directory.
mkdir -p build
cd build

cmake ..
make VERBOSE=1 -j2 catch_test
./catch_test
