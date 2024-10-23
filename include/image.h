#ifndef __IMAGE_H
#define __IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 

typedef struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct Image {
    // height and width are in pixels
    unsigned int height; // rows
    unsigned int width; // cols
    unsigned char max_intensity;
    // upper-left corner has position <row #0, column #0> 
    // and the lower-right corner has position <row #(H-1), column #(W-1)>
    // altneratively, 'raster' could be stored as a 1D row-major order array with helper functions and memory allocated dynamically
    Pixel** raster;
} Image;

Image *load_image(char *filename);
void delete_image(Image *image);
unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col);
unsigned short get_image_width(Image *image);
unsigned short get_image_height(Image *image);

unsigned int hide_message(char *message, char *input_filename, char *output_filename);
char *reveal_message(char *input_filename);
unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename);
void reveal_image(char *input_filename, char *output_filename);

// my functions
char *get_file_extension(char *filename);
bool check_file_exists(char *filename);
void file_skip_comments(FILE *fp);

#endif // __IMAGE_H