#include "qtree.h"

QTNode *create_quadtree(Image *image, double max_rmse) {
    return create_quadtree_helper(image, max_rmse, 0, 0, image->height, image->width);
}

QTNode *create_quadtree_helper(Image *image, double max_rmse, int row, int col, int height, int width) {
    int row_end = height;
    int col_end = width;

    double rmse = compute_rmse(image, row, col, row_end, col_end);
    
    QTNode *node = malloc(sizeof(QTNode));
    node->intensity = compute_average_intensity(image, row, col, row_end, col_end);
    node->row = row;
    node->col = col;
    node->height = row_end - row;
    node->width = col_end - col;

    if (node == NULL) {
        printf("create_quadtree_helper(): Memory allocation error...\n");
        return NULL;
    }

    if (row_end - row == 1 && col_end - col == 1) {
        node->child1 = node->child2 = node->child3 = node->child4 = NULL;
        return node;
    }
    if (rmse <= max_rmse) {
        node->child1 = node->child2 = node->child3 = node->child4 = NULL;
        return node;
    }

    // midpoint formulas
    int row_mid = (row_end + row) / 2;
    int col_mid = (col_end + col) / 2;

    if (row_end - row == 1) {
        node->child3 = node->child4 = NULL;

        node->child1 = create_quadtree_helper(image, max_rmse, row, col, row_end, col_mid);  // Top-left
        node->child2 = create_quadtree_helper(image, max_rmse, row, col_mid, row_end, col_end);  // Top-right
        return node;
    }
    else if (col_end - col == 1) {
        node->child2 = node->child4 = NULL;

        node->child1 = create_quadtree_helper(image, max_rmse, row, col, row_mid, col_end);  // Top-left
        node->child3 = create_quadtree_helper(image, max_rmse, row_mid, col, row_end, col_end);  // Bottom-left
        return node;
    }

    node->child1 = create_quadtree_helper(image, max_rmse, row, col, row_mid, col_mid);      // Top-left
    node->child2 = create_quadtree_helper(image, max_rmse, row, col_mid, row_mid, col_end);    // Top-right
    node->child3 = create_quadtree_helper(image, max_rmse, row_mid, col, row_end, col_mid);   // Bottom-left
    node->child4 = create_quadtree_helper(image, max_rmse, row_mid, col_mid, row_end, col_end); // Bottom-right

    return node;
}

double compute_rmse(Image *image, int row, int col, int height, int width) {
    double average = compute_average_intensity(image, row, col, height, width);
    int total_pixels = (height - row) * (width - col);
    
    double sum = 0.0;
    for (int i = row; i < height; i++) {
        for (int j = col; j < width; j++) {
            double pixel = (double) image->raster[i][j].red;
            sum += pow(pixel - average, 2);
        }
    }

    double rmse = sqrt(sum / (double) total_pixels);
    return rmse;
}

unsigned char compute_average_intensity(Image *image, int row, int col, int height, int width) {
    double average = 0.0;
    int total_pixels = (height - row) * (width - col);
    for (int i = row; i < height; i++) {
        for (int j = col; j < width; j++) {
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
    else if (node->child2 == NULL) {
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
    // we assume that the node poointer is valid.
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
    FILE *fp = fopen(filename, "w");
    save_preorder_qt_helper(root, fp);
    fclose(fp);
}

void save_preorder_qt_helper(QTNode *root, FILE *fp) {
    if (root == NULL) {
        return;
    }

    char *qt_node_string = stringify_qt_node(root);
    if (qt_node_string != NULL) {
        file_print_line(qt_node_string, fp);
        //printf("%s\n",qt_node_string);
        free(qt_node_string); // need to free() since qt_node_string was malloc'd
    }

    // Recursively process child nodes
    
    // left
    save_preorder_qt_helper(get_child1(root), fp);
    save_preorder_qt_helper(get_child2(root), fp);
    // right
    save_preorder_qt_helper(get_child3(root), fp);
    save_preorder_qt_helper(get_child4(root), fp);
}

char *stringify_qt_node(QTNode *node) {
    // need to malloc here
    char *string = malloc(40 * sizeof(char));
    if (node->child1 == NULL && node->child2 == NULL && node->child3 == NULL && node->child4 == NULL) {
        strcpy(string, "L ");
    }
    else {
        strcpy(string, "N ");
    }

    char buffer[40];
    snprintf(buffer, sizeof(buffer), "%hhu %d %d %d %d", node->intensity, node->row, node->height, node->col, node->width);
    strcat(string, buffer);
    return string;
}

void file_print_line(char *line, FILE *fp) {
    fprintf(fp, "%s\n", line);
}