#!/bin/bash

output_dir=build
CxxCompiler=clang++

if [ ! -z "$1" ]; then
    output_dir=$1
fi

if [ ! -z "$2" ]; then
    CxxCompiler=$2
fi

if [ ! -f "CMakeLists.txt" ]; then
    echo "This script must be run at the project root"
    exit 1
fi

echo "Setting up cmake"
echo "cmake -S . -B $output_dir -DCMAKE_CXX_COMPILER=$CxxCompiler"
cmake -S . -B $output_dir -DCMAKE_CXX_COMPILER=$CxxCompiler

if [ -f "compile_commands.json" ]; then
    exit 0
fi

echo "Symlinking compile_commands.json"
ln -s $output_dir/compile_commands.json .
