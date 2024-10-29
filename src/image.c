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

    fprintf(fp, "P3\n%u %u\n%u\n", width, height, max_intensity);

    int counter = total_encodable_msg_chars;
    int pixel_index = 0;
    int msg_char_index = 0;

    while (counter > 0) {
        unsigned char pixel[8];
        for (int i = 0; i < 8; i++) {
            pixel[i] = get_pixel_from_row_major_index(image, pixel_index++);
        }

        // 8-bit ASCII code for the null byte - 00000000 (aka NUL)

        unsigned char character = (counter == 1) ? '\0' : (unsigned char) message[msg_char_index];

        for (int i = 0; i < 8; i++) {
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
        printf("get_pixel_from_row_major_index(): Index %u is too big.\n", index);
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

        for (int i = 0; i < 8; i++) {
            pixel[i] = get_pixel_from_row_major_index(image, pixel_index++);
        }

        for (int i = 0; i < 8; i++) {
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
    if (!check_file_exists(secret_image_filename)) {
        printf("hide_image(): secret_image_filename does not exist.\n");
        return 10;
    }
    if (!check_file_exists(input_filename)) {
        printf("hide_image(): input_filename does not exist.\n");
        return 10;
    }

    Image *secret_image = load_image(secret_image_filename);
    Image *input_image = load_image(input_filename);

    if (!secret_image || !input_image) {
        printf("hide_image(): Failed to load images.\n");
        delete_image(secret_image);
        delete_image(input_image);
        return 10;
    }

    int secret_image_total_pixels = get_image_width(secret_image) * get_image_height(secret_image);
    int input_image_total_pixels = get_image_width(input_image) * get_image_height(input_image);
    int secret_image_total_pixels_needed = 16 + (8 * secret_image_total_pixels);

    if (secret_image_total_pixels_needed > input_image_total_pixels) {
        printf("hide_image(): Input image too small to encode secret image.\n");
        delete_image(secret_image);
        delete_image(input_image);
        return 0;
    }

    FILE *fp = fopen(output_filename, "w");

    fprintf(fp, "P3\n");
    fprintf(fp, "%u %u\n", input_image->width, input_image->height);
    fprintf(fp, "%u\n", input_image->max_intensity);

    int secret_pixel_index = 0;
    int input_pixel_index = 0;

    // encode width
    for (int i = 0; i < 8; i++) {
        int secret_pixel_width = get_image_width(secret_image);
        unsigned char input_pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);

        unsigned char bit = (secret_pixel_width >> (7 - i)) & 1;
        unsigned char new_pixel = (input_pixel & ~1) | bit;
        fprintf(fp, "%hhu %hhu %hhu\n", new_pixel, new_pixel, new_pixel);
    }

    // encode height
    for (int i = 0; i < 8; i++) {
        int secret_pixel_height = get_image_height(secret_image);
        unsigned char input_pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);

        unsigned char bit = (secret_pixel_height >> (7 - i)) & 1;
        unsigned char new_pixel = (input_pixel & ~1) | bit;
        fprintf(fp, "%hhu %hhu %hhu\n", new_pixel, new_pixel, new_pixel);
    }

    // encode remaining secret pixels
    for (int i = 0; i < secret_image_total_pixels; i++) {
        unsigned char pixel_to_encode = get_pixel_from_row_major_index(secret_image, secret_pixel_index++);

        for (int j = 0; j < 8; j++) {
            unsigned char input_pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);

            unsigned char bit = (pixel_to_encode >> (7 - j)) & 1;
            unsigned char new_pixel = (input_pixel & ~1) | bit;
            fprintf(fp, "%hhu %hhu %hhu\n", new_pixel, new_pixel, new_pixel);
        }
    }

    // print remaining pixels from 'input_image' that were not encoded with secret pixels
    while (input_pixel_index < input_image_total_pixels) {
        unsigned char pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);
        fprintf(fp, "%hhu %hhu %hhu\n", pixel, pixel, pixel);
    }

    delete_image(secret_image);
    delete_image(input_image);
    fclose(fp);

    return 1;
}

void reveal_image(char *input_filename, char *output_filename) {
    if (!check_file_exists(input_filename)) {
        printf("reveal_image(): Input file name does not exist.\n");
        return;
    }
    Image *input_image = load_image(input_filename);
    FILE *fp_output = fopen(output_filename, "w");

    if (!input_image || !input_image->raster) {
        printf("reveal_image(): input image did not load properly.\n");
        return;
    }

    int input_pixel_index = 0;

    int secret_image_width = 0;
    int secret_image_height = 0;
    int secret_image_size = 0;

    // decode width
    for (int i = 0; i < 8; i++) {
        unsigned char input_pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);
        char bit = input_pixel & 1;
        secret_image_width = secret_image_width | (bit << (7 - i));
    }

    // decode height
    for (int i = 0; i < 8; i++) {
        unsigned char input_pixel = get_pixel_from_row_major_index(input_image, input_pixel_index++);
        char bit = input_pixel & 1;
        secret_image_height = secret_image_height | (bit << (7 - i));
    }

    fprintf(fp_output, "P3\n");
    fprintf(fp_output, "%u %u\n", secret_image_width, secret_image_height);
    fprintf(fp_output, "%u\n", input_image->max_intensity);

    secret_image_size = secret_image_width * secret_image_height;

    // decode pixels
    for (int i = 0; i < secret_image_size; i++) {
        unsigned char decoded_secret_pixel = 0;

        for (int j = 0; j < 8; j++) {
            unsigned char pixel_to_decode = get_pixel_from_row_major_index(input_image, input_pixel_index++);
            char bit = pixel_to_decode & 1;
            decoded_secret_pixel = decoded_secret_pixel | (bit << (7 - j));
        }
        fprintf(fp_output, "%hhu %hhu %hhu\n", decoded_secret_pixel, decoded_secret_pixel, decoded_secret_pixel);

    }

    // done - don't need the non-encoded pixels from input image

    fclose(fp_output);
    delete_image(input_image);
}
