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

# Build and run everything.
if [ "$TRAVIS_OS_NAME" = osx ]; then
	cmake -G Xcode ..
	xcodebuild
	Debug/catch_test
else
	cmake ..
	make VERBOSE=1 -j2 catch_test
	./catch_test
fi
