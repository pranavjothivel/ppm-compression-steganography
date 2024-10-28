#include "image.h"

Image *load_image(char *filename) {
    char *ext = get_file_extension(filename);
    if (ext == NULL || strcmp(ext, "ppm") != 0) {
        printf("load_image(): File provided does not have .ppm extension... ");
        return NULL;
    }
    if (!check_file_exists(filename)) {
        printf("load_image(): Filename is null or does not exist... ");
        return NULL;
    }

    Image* image = malloc(sizeof(Image));
    image->width = 0;
    image->height = 0;
    image->max_intensity = 0;

    // fp is file handle
    FILE *fp = fopen(filename, "r");
    
    // remember: null terminator 
    char magic_number[3];
    fscanf(fp, "%2s", magic_number);

    if (strcmp(magic_number, "P3") != 0) {
        fclose(fp);
        printf("load_image(): Missing magic number of P3...");
        return NULL;
    }

    file_skip_comments(fp);
    fscanf(fp, "%u %u", &image->width, &image->height);

    file_skip_comments(fp);
    fscanf(fp, "%hhu", &image->max_intensity);
    
    image->raster = malloc(image->height * sizeof(Pixel*));

    for (unsigned int i = 0; i < image->height; i++) {
        image->raster[i] = malloc(image->width * sizeof(Pixel));

        for (unsigned int j = 0; j < image->width; j++) {
            // red = blue = green
            unsigned char red;
            unsigned char blue;
            unsigned char green;

            file_skip_comments(fp);
            fscanf(fp, "%hhu %hhu %hhu", &red, &green, &blue);
            file_skip_comments(fp);

            image->raster[i][j].red = red;
            image->raster[i][j].green = green;
            image->raster[i][j].blue = blue;
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

void file_skip_comments(FILE *fp) {
    char character;
    
    while ((character = fgetc(fp)) != EOF) {
        if (character == '#') {
            while ((character = fgetc(fp)) != EOF && character != '\n');
        } 
        else if (!isspace(character)) {
            ungetc(character, fp);
            break;
        }
    }
}

void delete_image(Image *image) {
    if (image == NULL) {
        printf("delete_image(): Image is null. ");
        return;
    }
    for (unsigned int i = 0; i < image->height; i++) {
        free(image->raster[i]);
    }
    
    free(image->raster);
    image->raster = NULL;
    free(image);
    image = NULL;
}

unsigned short get_image_width(Image *image) {
    if (image == NULL) {
        printf("get_image_width(): Invalid image pointer. ");
        return 0;
    }
    return image->width;
}

unsigned short get_image_height(Image *image) {
    if (image == NULL) {
        printf("get_image_width(): Invalid image pointer. ");
        return 0;
    }
    return image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    // if (image == NULL || row >= image->height || col >= image->width) {
    //     printf("get_image_intensity(): Image is null, or row is out of bounds, or col is out of bounds. ");
    //     return 0;
    // }
    if (image == NULL) {
        printf("get_image_intensity(): Image is null. ");
        return 0;
    }
    if (row >= image->height) {
        printf("get_image_intensity(): Row is out of bounds. ");
        return 0;
    }
    if (col >= image->width) {
        printf("get_image_intensity(): Col is out of bounds. ");
        return 0;
    }
    unsigned char intensity = image->raster[row][col].red;
    return intensity;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    unsigned int counter = 0;
    (void)message;
    (void)input_filename;
    (void)output_filename;
    return counter;
}

char *reveal_message(char *input_filename) {
    Image *img = load_image(input_filename);
    int size = (get_image_width(img)) * (get_image_height(img));
    delete_image(img);

    char *message = malloc((8 * size) + 1);
    message[0] = '\0';

    FILE *fp = fopen(input_filename, "r");

    // remember: null terminator 
    char magic_number[3];
    fscanf(fp, "%2s", magic_number);

    unsigned int i1, i2;
    unsigned char temp_buffer;

    file_skip_comments(fp);
    fscanf(fp, "%u %u", &i1, &i2);

    file_skip_comments(fp);
    fscanf(fp, "%hhu", &temp_buffer);

    unsigned char pixel[8];
    while ((fscanf(fp, "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu", &pixel[0], &pixel[1], &pixel[2], &pixel[3], &pixel[4], &pixel[5], &pixel[6], &pixel[7])) == 8) {
        char ch = 0;

        for (unsigned short i = 0; i < 8; i++) {
            unsigned char lsb = pixel[i] & 1;
            ch |= (lsb << i);
        }

        char ch_as_str[2];
        ch_as_str[0] = ch;
        ch_as_str[1] = '\0';

        // strncat(message, ch_as_str, sizeof(ch_as_str));
        strcat(message, ch_as_str);

        if (ch == '\0') {
            break;
        }
    }

    fclose(fp);

    return message;
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