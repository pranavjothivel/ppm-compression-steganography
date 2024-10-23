#include "image.h"

Image *load_image(char *filename) {
    char *ext = get_file_extension(filename);
    if (ext == NULL || strcmp(ext, "ppm") != 0) {
        printf("File provided does not have .ppm extension...");
        return NULL;
    }
    if (!check_file_exists(filename)) {
        printf("Filename is null or does not exist...");
        return NULL;
    }

    Image* image = malloc(sizeof(Image));
    // fp is file handle
    FILE *fp = fopen(filename, "r");
    
    // remember: null terminator 
    char magic_number[3];
    fscanf(fp, "%2s", magic_number);
    if (strcmp(magic_number, "P3") != 0) {
        fclose(fp);
        printf("Missing magic number of P3...");
        return NULL;
    }

    fscanf(fp, "%u %u", &image->width, &image->height);
    fscanf(fp, "%c", &image->max_intensity);
    image->raster = malloc(image->height * sizeof(Pixel*));
    for (unsigned int i = 0; i < image->height; i++) {
        for (unsigned int j = 0; j < image->width; j++) {
            unsigned char value;
            fscanf(fp, "%c", &value);
            image->raster[i][j].red = value;
            image->raster[i][j].green = value;
            image->raster[i][j].blue = value;
        }
    }

    return image;
}

char *get_file_extension(char *filename) {
    char *ext = strrchr(filename, '.');
    if (ext == NULL) {
        return NULL;
    }
    else {
        // increment pointer by 1 to return file extension without '.'
        return ext + 1;
    }
}

bool check_file_exists(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return false;
    }
    else {
        fclose(fp);
        return true;
    }
}

void delete_image(Image *image) {
    // free each raster
    free(image->raster);
    free(image);
}

unsigned short get_image_width(Image *image) {
    if (image == NULL) {
        printf("Invalid image pointer...");
        return 0;
    }
    return image->width;
}

unsigned short get_image_height(Image *image) {
    if (image == NULL) {
        printf("Invalid image pointer...");
        return 0;
    }
    return image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    (void)image;
    (void)row;
    (void)col;
    return 0;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    (void)message;
    (void)input_filename;
    (void)output_filename;
    return 0;
}

char *reveal_message(char *input_filename) {
    (void)input_filename;
    return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    (void)secret_image_filename;
    (void)input_filename;
    (void)output_filename;
    return 10;
}

void reveal_image(char *input_filename, char *output_filename) {
    (void)input_filename;
    (void)output_filename;
}