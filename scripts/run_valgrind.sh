#!/bin/bash

# Check for the --keep-output argument
KEEP_OUTPUT=false
for arg in "$@"; do
    if [ "$arg" == "--keep-output" ]; then
        KEEP_OUTPUT=true
        printf "INFO: Keeping 'tests/output/' directory...\n"

    fi
done

# Run CMake build
printf "INFO: Building the project...\n\n"
cmake -S . -B build
cmake --build build
rm -rf tests/output/

# Check if the build succeeded
if [ $? -ne 0 ]; then
    printf "\n\nINFO: Build failed. Exiting.\n\n"
    exit 1
fi

# Run Valgrind with leak checks and other diagnostics
printf "\n\nINFO: Running Valgrind on hw3_main...\n\n"
valgrind_output=$(mktemp) # Create a temporary file for Valgrind output

valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes \
         --error-exitcode=37 ./build/hw3_main &> "$valgrind_output"

# Check if Valgrind found any issues
if [ $? -eq 37 ]; then
    printf "\nERROR: Valgrind detected memory issues:\n"
    cat "$valgrind_output" # Show the full Valgrind output if there are issues
    rm -f "$valgrind_output" # Clean up the temporary file
    exit 37
else
    printf "\nINFO: Valgrind finished without issues.\n\n"
    printf "**********************************************************\n"
    printf "INFO: Running ./build/hw3_main...\n\n"
    ./build/hw3_main
fi

printf "\n*****************************************************\n"
printf "INFO: ./build/hw3_main complete...\n"

rm -f "$valgrind_output" # Clean up the temporary file

# Conditionally clean up test output directory
if [ "$KEEP_OUTPUT" = false ]; then
    printf "INFO: Removing tests/output/ directory...\n"
    rm -rf tests/output/
else
    printf "INFO: Skipping removal of tests/output/ directory.\n"
fi

printf "INFO: Removing vgcore.* files...\n"
rm -f vgcore.*

printf "\nINFO: Script completed.\n"