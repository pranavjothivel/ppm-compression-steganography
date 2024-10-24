#ifndef QTREE_H
#define QTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 

// QTNode children
// 1 2
// 3 4
typedef struct QTNode {
    unsigned char intensity;
    int row, col; // starting top-left position
    int height, width; // <row + height - 1, col + width - 1> is the ending bottom-right position
    struct QTNode *child1;
    struct QTNode *child2;
    struct QTNode *child3;
    struct QTNode *child4;
} QTNode;

QTNode *create_quadtree(Image *image, double max_rmse);  
QTNode *get_child1(QTNode *node);
QTNode *get_child2(QTNode *node);
QTNode *get_child3(QTNode *node);
QTNode *get_child4(QTNode *node);
unsigned char get_node_intensity(QTNode *node);
void delete_quadtree(QTNode *root);
void save_qtree_as_ppm(QTNode *root, char *filename); 
QTNode *load_preorder_qt(char *filename);
void save_preorder_qt(QTNode *root, char *filename);  

// my functions
unsigned char compute_rmse(Image *image, int row, int width, int col, int height);
double compute_average_intensity(Image *image, int row, int width, int col, int height);
QTNode *create_quadtree_helper(Image *image, double max_rmse, int row, int col, int row_end, int col_end);

#endif // QTREE_H