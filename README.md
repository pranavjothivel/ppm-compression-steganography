# CSE 220 - Homework 03  
Pranav Jothivel

## Checklist  
- [x] Part 1 Implementation  
- [x] Part 1 Unit Tests (incl. Memory Tests with Valgrind)  
- [x] Part 2 Implementation  
- [x] Part 2 Unit Tests (incl. Memory Tests with Valgrind)  
  - [x] Test 1: RMSE Calculation Accuracy  
  - [x] Test 2: Average Intensity Calculation (No issue with Test 1, indicates this function is running as expected.)  
  - [x] Test 3: Quadtree Creation Validity  
  - [x] Test 4: Edge Case (1x1 Image)  
- [x] Part 3 Implementation  
- [x] Part 3 Unit Tests (incl. Memory Tests with Valgrind)  

## AutoTest  
- [x] `load_image and delete_image tests`
- [x] `unit_tests_create_quadtree tests`
- [x] `unit_tests_load_preorder_qt tests`
- [x] `unit_tests_save_preorder_qt tests`
- [x] `unit_tests_save_qtree_as_ppm tests`
- [x] `unit_tests_hide_message tests`
- [x] `unit_tests_reveal_message tests`
- [x] `unit_tests_hide_image tests`
- [x] `unit_tests_reveal_image tests`
- [x] `unit_tests_memory tests`

## Running Valgrind  
```bash
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw3_main
```

## Running Test Suite (using script)  
To execute the test suite, follow these steps:

1. Ensure the script is executable:
   ```bash
   chmod +x ./scripts/run_tests.sh
   ```

2. Run the tests using the script. You can control whether Valgrind runs by passing the `--no-valgrind` flag. Here's an example:
   ```bash
   ./scripts/run_tests.sh --no-valgrind
   ```

### Script Flags  
- **`--keep-output`**: Retains the `tests/output/` directory after running the tests.  
- **`--no-valgrind`**: Skips Valgrind memory checks for faster execution.  
- **Test-specific flags**:
  - `--test-load-image`  
  - `--test-rmse`  
  - `--test-create-quadtree`  
  - `--test-load-preorder-qt`  
  - `--test-save-preorder-qt`  
  - `--test-save-qtree-as-ppm`  
  - `--test-hide-reveal-msg`  
  - `--test-hide-reveal-img`

### Example Usage  
To run the tests for `load_image` with Valgrind:
```bash
./scripts/run_tests.sh --test-load-image 
```

To skip Valgrind checks, run all tests, and keep the output directory:
```bash
./scripts/run_tests.sh --no-valgrind --keep-output
```
