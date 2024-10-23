# CSE 220 - Homework 03
Pranav Jothivel

## Checklist
- [ ] Part 1 Implementation
- [x] Part 1 Unit Tests
- [ ] Part 2 Implementation
- [ ] Part 2 Unit Tests
- [ ] Part 3 Implementation
- [ ] Part 3 Unit Tests

## Running Valgrind
```
valgrind --quiet -s --leak-check=full --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw3_main
```