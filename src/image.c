#include "image.h"

Image *load_image(char *filename) {
    char *ext = get_file_extension(filename);
    if (ext == NULL || strcmp(ext, "ppm") != 0) {
        printf("load_image(): File provided does not have .ppm extension...\n");
        return NULL;
    }
    if (!check_file_exists(filename)) {
        printf("load_image(): Filename is null or does not exist...\n");
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
        printf("load_image(): Missing magic number of P3...\n");
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
        printf("delete_image(): Image is null.\n");
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
        printf("get_image_width(): Invalid image pointer.\n");
        return 0;
    }
    return image->width;
}

unsigned short get_image_height(Image *image) {
    if (image == NULL) {
        printf("get_image_width(): Invalid image pointer.\n");
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
        printf("get_image_intensity(): Image is null.\n");
        return 0;
    }
    if (row >= image->height) {
        printf("get_image_intensity(): Row is out of bounds.\n");
        return 0;
    }
    if (col >= image->width) {
        printf("get_image_intensity(): Col is out of bounds.\n");
        return 0;
    }
    unsigned char intensity = image->raster[row][col].red;
    return intensity;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    if (!check_file_exists(input_filename)) {
        printf("hide_message(): input_filename does not exist.\n");
        return '\0';
    }

    Image *image = load_image(input_filename);

    int width = get_image_width(image);
    int height = get_image_height(image);
    int max_intensity = image->max_intensity;
    int size = width * height;

    int image_max_encodable_msg_chars = size / 8;
    // Includes null byte ('\0').
    int msg_chars_to_encode = strlen(message) + 1;
    int total_encodable_msg_chars = (msg_chars_to_encode > image_max_encodable_msg_chars) ? image_max_encodable_msg_chars : msg_chars_to_encode;
    
    FILE *fp = fopen(output_filename, "w");

    // 8-bit ASCII code for the null byte - 00000000 (aka NUL)

    fprintf(fp, "P3\n%u %u\n%u\n", width, height, max_intensity);

    int counter = total_encodable_msg_chars;
    int pixel_index = 0;
    int msg_char_index = 0;

    while (counter > 0) {
        unsigned char pixel[8];
        for (unsigned short i = 0; i < 8; i++) {
            pixel[i] = get_pixel_from_row_major_index(image, pixel_index++);
        }

        unsigned char character = (counter == 1) ? '\0' : (unsigned char) message[msg_char_index];

        for (unsigned short i = 0; i < 8; i++) {
            unsigned char bit = (character >> (7 - i)) & 1;
            unsigned char new_pixel = (pixel[i] & ~1) | bit;
            fprintf(fp, "%hhu %hhu %hhu\n", new_pixel, new_pixel, new_pixel);
        }


        msg_char_index++;
        counter--;
    }

    while (pixel_index < size) {
        unsigned char remaining_pixel = get_pixel_from_row_major_index(image, pixel_index++);
        fprintf(fp, "%hhu %hhu %hhu\n", remaining_pixel, remaining_pixel, remaining_pixel);
    }


    delete_image(image);
    fclose(fp);
    return total_encodable_msg_chars - 1;
}

unsigned char get_pixel_from_row_major_index(Image *image, int index) {
    int width = get_image_width(image);
    int height = get_image_height(image);
    int size = width * height;

    if (index >= size) {
        printf("get_pixel_from_row_major_index(): index too big.\n");
        return 0;
    }

    // Row-major indexing: index = (row * width) + col

    int row = (index / width);
    int col = (index % width);

    // Returns 'image->raster[row][col].red'. Does not matter because our images are grayscale so RGB all have same value.
    return image->raster[row][col].red;
}

char *reveal_message(char *input_filename) {
    if (!check_file_exists(input_filename)) {
        printf("reveal_message(): input_filename does not exist.\n");
        return NULL;
    }

    Image *image = load_image(input_filename);
    if (!image) {
        printf("reveal_message(): Error with image creation.\n");
        return NULL;
    }

    int width = get_image_width(image);
    int height = get_image_height(image);
    int size = width * height;

    int pixel_index = 0;

    char *message = malloc((size / 8));
    message[0] = '\0';
    
    while (pixel_index < size) {
        char character = '\0';
        unsigned char pixel[8];

        for (unsigned short i = 0; i < 8; i++) {
            pixel[i] = get_pixel_from_row_major_index(image, pixel_index++);
        }

        for (unsigned short i = 0; i < 8; i++) {
            char bit = pixel[i] & 1;
            character = character | (bit << (7 - i));
        }

        char character_as_string[2] = {character, '\0'};
        strncat(message, character_as_string, 2);

        if (character == '\0') {
            break;
        }
    }

    delete_image(image);
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