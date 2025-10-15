#!/bin/bash

# Create the build directory if it doesn't exist
mkdir -p build

# Compile the Supernova compiler
g++ src/main.cpp src/lexer.cpp src/parser.cpp -o build/supernova -std=c++11

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Supernova compiler built successfully!"
    echo "You can now run Nova source files using: ./build/supernova <your-file.nv>"
else
    echo "Error: Supernova compiler failed to build."
fi