# CSE 220 - Homework 03
Pranav Jothivel

## Checklist
- [x] Part 1 Implementation
- [x] Part 1 Unit Tests
- [ ] Part 2 Implementation
- [ ] Part 2 Unit Tests
- [ ] Part 3 Implementation
- [ ] Part 3 Unit Tests

## AutoTest
- [x] load_image and delete_image tests
- [ ] unit_tests_create_quadtree tests
- [ ] unit_tests_load_preorder_qt tests
- [ ] unit_tests_save_preorder_qt tests
- [ ] unit_tests_save_qtree_as_ppm tests
- [ ] unit_tests_hide_message tests
- [ ] unit_tests_reveal_message tests
- [ ] unit_tests_hide_image tests
- [ ] unit_tests_reveal_image tests
- [x] unit_tests_memory tests

## Running Valgrind
```
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw3_main
```