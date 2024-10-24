#include "qtree.h"

QTNode *create_quadtree(Image *image, double max_rmse) {
    return create_quadtree_helper(image, max_rmse, 0, 0, image->height, image->width);
}

QTNode *create_quadtree_helper(Image *image, double max_rmse, int row, int col, int row_end, int col_end) {
    // row_end is height
    // col_end is column
    unsigned char rmse = compute_rmse(image, row, row_end, col, col_end);
    if (rmse <= max_rmse) {
        return NULL;
    }

    // midpoint formulas
    unsigned int row_mid = (row + row_end) / 2;
    unsigned int col_mid = (col + col_end) / 2;

    QTNode *node = malloc(sizeof(QTNode));
    if (node == NULL) {
        printf("create_quadtree_helper(): Memory allocation error...\n");
        return NULL;
    }

    node->child1 = create_quadtree_helper(image, max_rmse, row, col, row_mid, col_mid);
    node->child2 = create_quadtree_helper(image, max_rmse, row, col_mid, row_mid, col_end);   // Top-right
    node->child3 = create_quadtree_helper(image, max_rmse, row_mid, col, row_end, col_mid);   // Bottom-left
    node->child4 = create_quadtree_helper(image, max_rmse, row_mid, col_mid, row_end, col_end); // Bottom-right

    return node;
}

unsigned char compute_rmse(Image *image, int row, int width, int col, int height) {
    double average = compute_average_intensity(image, row, width, col, height);
    int total_pixels = width * height;
    
    double sum = 0.0;
    for (unsigned short i = row; i < row + width; i++) {
        for (unsigned short j = col; j < col + height; j++) {
            double pixel = (double) image->raster[i][j].red;
            sum += pow(pixel - average, 2);
        }
    }

    double rmse = sqrt(sum / (double) total_pixels);
    return (unsigned char) rmse;
}

double compute_average_intensity(Image *image, int row, int width, int col, int height) {
    double average = 0.0;
    int total_pixels = width * height;
    for (unsigned short i = row; i < row + width; i++) {
        for (unsigned short j = col; j < col + height; j++) {
            average += image->raster[i][j].red;
        }
    }
    average = average / (double) total_pixels;
    return average;
}

QTNode *get_child1(QTNode *node) {
    if (node == NULL) {
        printf("get_child1(): node is null.\n");
        return NULL;
    }
    else if (node->child1 == NULL) {
        printf("get_child1(): child is null.\n");
        return NULL;
    }
    return node->child1;
}

QTNode *get_child2(QTNode *node) {
    if (node == NULL) {
        printf("get_child2(): node is null.\n");
        return NULL;
    }
    else if (node->child1 == NULL) {
        printf("get_child2(): child is null.\n");
        return NULL;
    }
    return node->child2;
}

QTNode *get_child3(QTNode *node) {
    if (node == NULL) {
        printf("get_child3(): node is null.\n");
        return NULL;
    }
    else if (node->child3 == NULL) {
        printf("get_child3(): child is null.\n");
        return NULL;
    }
    return node->child3;
}

QTNode *get_child4(QTNode *node) {
    if (node == NULL) {
        printf("get_child4(): node is null.\n");
        return NULL;
    }
    else if (node->child4 == NULL) {
        printf("get_child4(): child is null.\n");
        return NULL;
    }
    return node->child4;
}

unsigned char get_node_intensity(QTNode *node) {
    if (node == NULL) {
        printf("get_node_intensity(): node is null.\n");
        return 0;
    }
    return node->intensity;
}

void delete_quadtree(QTNode *root) {
    if (root == NULL) {
        return;
    }
    delete_quadtree(root->child1);
    delete_quadtree(root->child2);
    delete_quadtree(root->child3);
    delete_quadtree(root->child4);
    free(root);
}

void save_qtree_as_ppm(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

QTNode *load_preorder_qt(char *filename) {
    (void)filename;
    return NULL;
}

void save_preorder_qt(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}
