#!/bin/bash

# Initialize variables
KEEP_OUTPUT=false
RUN_ALL=true  # Default to running all tests if no specific test flags are passed
TEST_FLAGS=""
PROVIDED_FLAGS=()  # Array to store provided flags

# Parse command-line arguments for test flags
for arg in "$@"; do
    case $arg in
        --keep-output)
            KEEP_OUTPUT=true
            PROVIDED_FLAGS+=("--keep-output")
            printf "INFO: Keeping 'tests/output/' directory...\n"
            ;;
        --test-load-image)
            TEST_FLAGS+=" test_load_image=true;"
            PROVIDED_FLAGS+=("--test-load-image")
            RUN_ALL=false  # Disable running all tests by default
            ;;
        --test-rmse)
            TEST_FLAGS+=" test_rmse=true;"
            PROVIDED_FLAGS+=("--test-rmse")
            RUN_ALL=false
            ;;
        --test-create-quadtree)
            TEST_FLAGS+=" test_create_quadtree=true;"
            PROVIDED_FLAGS+=("--test-create-quadtree")
            RUN_ALL=false
            ;;
        --test-load-preorder-qt)
            TEST_FLAGS+=" test_load_preorder_qt=true;"
            PROVIDED_FLAGS+=("--test-load-preorder-qt")
            RUN_ALL=false
            ;;
        --test-save-preorder-qt)
            TEST_FLAGS+=" test_save_preorder_qt=true;"
            PROVIDED_FLAGS+=("--test-save-preorder-qt")
            RUN_ALL=false
            ;;
        --test-save-qtree-as-ppm)
            TEST_FLAGS+=" test_save_qtree_as_ppm=true;"
            PROVIDED_FLAGS+=("--test-save-qtree-as-ppm")
            RUN_ALL=false
            ;;
        --test-hide-reveal-msg)
            TEST_FLAGS+=" test_hide_reveal_msg=true;"
            PROVIDED_FLAGS+=("--test-hide-reveal-msg")
            RUN_ALL=false
            ;;
        --test-hide-reveal-img)
            TEST_FLAGS+=" test_hide_reveal_img=true;"
            PROVIDED_FLAGS+=("--test-hide-reveal-img")
            RUN_ALL=false
            ;;
    esac
done

# If no specific test flags were passed, enable all tests
if [ "$RUN_ALL" = true ]; then
    TEST_FLAGS=" test_load_image=true; test_rmse=true; test_create_quadtree=true; \
                 test_load_preorder_qt=true; test_save_preorder_qt=true; \
                 test_save_qtree_as_ppm=true; test_hide_reveal_msg=true; \
                 test_hide_reveal_img=true;"
    printf "INFO: No specific test flags provided. Running all tests...\n"
else
    printf "INFO: Provided flags: %s\n" "${PROVIDED_FLAGS[*]}"
fi

# Run CMake build
printf "INFO: Building the project...\n\n"
cmake -S . -B build
cmake --build build

printf "INFO: Removing 'tests/output/' directory...\n"
rm -rf tests/output/

# Check if the build succeeded
if [ $? -ne 0 ]; then
    printf "\n\nINFO: Build failed. Exiting.\n\n"
    exit 1
fi

# Run Valgrind with dynamically set test flags
printf "\n\nINFO: Running Valgrind with unit test flags...\n\n"
valgrind_output=$(mktemp) # Create a temporary file for Valgrind output

# Use Valgrind to execute the binary with the test flags passed as environment variables
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes \
         --error-exitcode=37 env $TEST_FLAGS ./build/hw3_main &> "$valgrind_output"

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

# Conditionally clean up the tests/output directory
if [ "$KEEP_OUTPUT" = false ]; then
    printf "INFO: REMOVING tests/output/ directory...\n"
    rm -rf tests/output/
else
    printf "INFO: SKIPPING removal of tests/output/ directory.\n"
fi

# Clean up any vgcore.* files
printf "INFO: Removing vgcore.* files...\n"
rm -f vgcore.*

printf "\nINFO: Script completed.\n"