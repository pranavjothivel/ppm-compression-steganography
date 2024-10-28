#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

#define BUFFER_SIZE 8192

#include "qtree.h"

void copy_file(char *src_filename, char *dest_filename);
void prepare_input_image_file(char *image_filename);

// my functions
void run_load_image_unit_test(char *filename, short i);
void run_rmse_unit_test(char *filename, short i);
bool run_load_preorder_unit_test(char *filename, int i);
bool compare_files(const char *file1, const char *file2);