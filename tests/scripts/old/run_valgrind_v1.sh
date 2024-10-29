#!/bin/bash

# Run CMake build
printf "\n\nBuilding the project...\n\n"
cmake --build build

# Check if the build succeeded
if [ $? -ne 0 ]; then
    printf "\n\nBuild failed. Exiting.\n\n"
    exit 1
fi

# Run Valgrind with leak checks and other diagnostics
printf "\n\nRunning Valgrind on hw3_main...\n\n"
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw3_main

# Check if Valgrind found any issues
if [ $? -eq 37 ]; then
    printf "\nValgrind detected memory issues.\n"
    exit 37
else
    printf "\nValgrind finished without issues.\n"
fi

# Clean up test output directory
printf "\nRemoving tests/output/ directory...\n"
rm -rf tests/output/

printf "\nRemoving vgcore.* files...\n"
rm -f vgcore.*

printf "\nScript completed.\n"
