# CSE 220 - Homework 03
Pranav Jothivel

## Checklist
- [x] Part 1 Implementation
- [x] Part 1 Unit Tests (incl. Memory Tests with Valgrind)
- [x] Part 2 Implementation
- [x] Part 2 Unit Tests (incl. Memory Tests with Valgrind)
  - [x] Test 1: RMSE Calculation Accuracy
  - [x] Test 2: Average Intensity Calculation (No issue with Test 1, indicates this function is running as expected.)
  - [X] Test 3: Quadtree Creation Validity
  - [X] Test 4: Edge Case (1x1 Image)
- [ ] Part 3 Implementation
- [ ] Part 3 Unit Tests (incl. Memory Tests with Valgrind)

## AutoTest
- [x] load_image and delete_image tests
- [x] unit_tests_create_quadtree tests
- [x] unit_tests_load_preorder_qt tests
- [x] unit_tests_save_preorder_qt tests
- [x] unit_tests_save_qtree_as_ppm tests
- [x] unit_tests_hide_message tests
- [x] unit_tests_reveal_message tests
- [x] unit_tests_hide_image tests
- [ ] unit_tests_reveal_image tests
- [x] unit_tests_memory tests

## Running Valgrind
```
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw3_main
```

## Running Test Suite (using script)
```
chmod +X ./build/run_valgrind.sh
```
```
./build/run_valgrind.sh --keep-output
```