#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Run CMake with vcpkg toolchain
cmake -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake ..

# Build the project
make

# Run tests
ctest

# Generate documentation
cd ..
doxygen Doxyfile
