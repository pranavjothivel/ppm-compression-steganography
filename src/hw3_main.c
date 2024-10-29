#include "qtree.h"
#include "image.h"

#include "tests_utils.h"

// // test case flags
// bool test_load_image = false;        // passing valgrind (10/24)
// bool test_rmse = false;              // passing valgrind (10/24)
// bool test_create_quadtree = false;   // passing valgrind and codegrade (10/25)
// bool test_load_preorder_qt = true;        // passing valgrind and codegrade (10/28)   
// bool test_save_preorder_qt = false;  // passing valgrind and codegrade (10/25)
// bool test_save_qtree_as_ppm = false;
// bool test_hide_reveal_msg = false;
// bool test_hide_reveal_img = false;

// unit test flags (default to false) - used with run_valgrind shell script
bool test_load_image = false;
bool test_rmse = false;
bool test_create_quadtree = false;
bool test_load_preorder_qt = false;
bool test_save_preorder_qt = false;
bool test_save_qtree_as_ppm = false;
bool test_hide_reveal_msg = false;
bool test_hide_reveal_img = false;

void set_test_flags_from_env() {
    if (getenv("test_load_image")) test_load_image = true;
    if (getenv("test_rmse")) test_rmse = true;
    if (getenv("test_create_quadtree")) test_create_quadtree = true;
    if (getenv("test_load_preorder_qt")) test_load_preorder_qt = true;
    if (getenv("test_save_preorder_qt")) test_save_preorder_qt = true;
    if (getenv("test_save_qtree_as_ppm")) test_save_qtree_as_ppm = true;
    if (getenv("test_hide_reveal_msg")) test_hide_reveal_msg = true;
    if (getenv("test_hide_reveal_img")) test_hide_reveal_img = true;
}

int main() {
    set_test_flags_from_env();
    struct stat st;
    if (stat("tests/output", &st) == -1)
        mkdir("tests/output", 0700);
    prepare_input_image_file("building1.ppm"); // copies the image to the images/ directory
    
    /******************************* prepare all images *******************************/
    printf("***** Start preparing input image files... *****\n\n");
    char *filenames[] = {
        "building1.ppm",
        "building2.ppm",
        "dog.ppm",
        "eagle.ppm",
        "einstein1.ppm",
        "einstein2.ppm",
        "i376.ppm",
        "lopsided.ppm",
        "mascot.ppm",
        "sbu1.ppm",
        "smileyface.ppm",
        "statue.ppm",
        "times_square.ppm",
        "tiny.ppm",
        "wolfie-tiny.ppm",
        "wolfie.ppm"
    };
    unsigned short filenames_len = sizeof(filenames) / sizeof(filenames[0]);
    for (unsigned short i = 0; i < filenames_len; i++) {
        prepare_input_image_file(filenames[i]);
    }
    printf("Done.\n\n");
    printf("***** End preparing input image files... *****\n\n");

    /******************************* load_image *******************************/
    if (test_load_image) {
        printf("***** Start load_image unit test(s)... *****\n\n");

        for (unsigned short i = 0; i < filenames_len; i++) {
            printf("(%d)\n", i + 1);

            // buffer
            char input_filename[64];
            strcpy(input_filename, "images/");
            strcat(input_filename, filenames[i]);

            run_load_image_unit_test(input_filename, i + 1);
        }
        // run_load_image_unit_test("einstein2.ppm", 5);

        printf("***** End load_image unit test(s)... *****\n\n");
    }

    /******************************* compute_rmse and compute_average_intensity *******************************/
    if (test_rmse) {
        printf("***** Start compute_rmse and compute_average_intensity unit test(s)... *****\n\n");

        for (unsigned short i = 0; i < filenames_len; i++) {
            printf("(%d)\n", i + 1);

            // buffer
            char input_filename[64];
            strcpy(input_filename, "images/");
            strcat(input_filename, filenames[i]);

            run_rmse_unit_test(input_filename, i + 1);
        }
        // run_load_image_unit_test("einstein2.ppm", 5);

        printf("***** End compute_rmse and compute_average_intensity unit test(s)... *****\n\n");
    }

    /******************************* common *******************************/
    double max_rmse = 25;
    Image *image = load_image("images/tiny.ppm");
    QTNode *root = create_quadtree(image, max_rmse);
    delete_image(image);
    delete_quadtree(root);

    /******************************* create_quadtree *******************************/
    
    if (test_create_quadtree) {
        printf("Creating quadtree...\n\n");
        root = create_quadtree(image, max_rmse);
        printf("Done with quadtree...\n\n");
        // See tests/input/load_preorder_qt1_qtree.txt for the expected results
        // You will need to write your own code to verify that your quadtree was constructed properly
        if (test_create_quadtree) {
            printf("***** Start create_quadtree unit test(s)... *****\n\n");
            
            printf("***** End create_quadtree unit test(s)... *****\n\n");
        }
        delete_quadtree(root);
        delete_image(image);
    }

    /******************************* load_preorder_qt *******************************/
    if (test_load_preorder_qt) {
        printf("***** Start load_preorder_qt unit test(s)... *****\n\n");

        for (unsigned short i = 0; i < filenames_len; i++) {
            printf("\n(%d)\n", i + 1);

            // buffer
            char input_filename[64];
            strcpy(input_filename, "images/");
            strcat(input_filename, filenames[i]);

            bool test = run_load_preorder_unit_test(input_filename, i + 1);
            if (test) {
                printf("Test for %s is CORRECT.\n", input_filename);
            }
            else {
                printf("Test for %s is INCORRECT.\n", input_filename);
            }
        }
        printf("\n(17)\n");

        QTNode *root1 = load_preorder_qt("tests/input/load_preorder_qt1_qtree.txt");
        save_preorder_qt(root1, "tests/output/save_load_preorder_qt1_qtree.txt");
        delete_quadtree(root1);

        if (true) {
            bool test = compare_files("tests/input/load_preorder_qt1_qtree.txt", "tests/output/save_load_preorder_qt1_qtree.txt");

            if (test) {
                printf("Test for 'tests/input/load_preorder_qt1_qtree.txt' is CORRECT.\n");
            }
            else {
                printf("Test for 'tests/input/load_preorder_qt1_qtree.txt' is INCORRECT.\n");
            }
        }
        printf("\n***** End load_preorder_qt unit test(s)... *****\n\n");
        
    }

    /******************************* save_preorder_qt *******************************/
    if (test_save_preorder_qt) {
        image = load_image("images/building1.ppm"); 
        root = create_quadtree(image, 25);
        // See tests/input/load_preorder_qt1_qtree.txt for expected output
        save_preorder_qt(root, "tests/output/save_preorder_qt1_qtree.txt");
        delete_quadtree(root);
        delete_image(image);
    }

    /******************************* save_qtree_as_ppm *******************************/
    if (test_save_qtree_as_ppm) {
        image = load_image("images/building1.ppm"); 
        root = create_quadtree(image, 25);
        save_qtree_as_ppm(root, "tests/output/save_qtree_as_ppm1.ppm");
        // See tests/expected/save_qtree_as_ppm1.ppm for the expected file.
        // Visual inspection is generally not sufficient to determine if your output image is correct.
        // You will need to write code to more rigorously check your output image for correctness.
        delete_image(image); 
        delete_quadtree(root);
    }

    /******************************* hide_message and reveal_message *******************************/
    if (test_hide_reveal_msg) {
        prepare_input_image_file("wolfie-tiny.ppm");
        hide_message("0000000000111111111122222222223333333333", "images/wolfie-tiny.ppm", "tests/output/hide_message1.ppm");
        char *message = reveal_message("tests/output/hide_message1.ppm");
        // char *message = reveal_message("images/smileyface.ppm");
        printf("Message: %s\n", message);
        free(message);
    }

    /******************************* hide_image and reveal_image *******************************/
    if (test_hide_reveal_img) {
        // hide_image("images/wolfie-tiny.ppm", "images/building1.ppm", "tests/output/hide_image1.ppm");
        // reveal_image("tests/output/hide_image1.ppm", "tests/output/reveal_image1.ppm");
        hide_image("images/eagle.ppm", "images/mascot.ppm", "tests/output/hide_image1.ppm");
        reveal_image("tests/output/hide_image1.ppm", "tests/output/reveal_image1.ppm");
        // if (hide_image("images/eagle.ppm", "images/building1.ppm", "tests/output/hide_image11.ppm") == 1) {
        //     reveal_image("tests/output/hide_image1.ppm", "tests/output/reveal_image1.ppm");
        // }
        // hide_image("images/eagle.ppm", "images/building1.ppm", "tests/output/hide_image11.ppm");
        // reveal_image("tests/output/hide_image11.ppm", "tests/output/reveal_image1.ppm");
    }

    return 0;
}

// my functions
void run_load_image_unit_test(char *filename, short i) {
    Image *image = load_image(filename);
    if (image) {
        printf("run_load_image_unit_test for %s\n", filename);

        unsigned int row = 0;
        unsigned int col = 0;

        printf("Height (Rows): %d, Width (Columns): %d, Intensity: %d\n", image->height, image->width, image->max_intensity);

        printf("Intensity at <0,0>: %d\n", get_image_intensity(image, row, col));
        printf("Intensity at <0,1>: %d\n", get_image_intensity(image, 0, 1));
        
        if (i != 14) {
            printf("Intensity at <5,5>: %d\n", get_image_intensity(image, 5, 5));
        }

        row = (image->height - 1) / 2;
        col = (image->width - 1) / 2;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = (image->height - 1) / 3;
        col = (image->width - 1) / 3;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = (image->height - 1) / 5;
        col = (image->width - 1) / 5;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = (image->height - 1) / 6;
        col = (image->width - 1) / 8;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = (image->height - 1) / 7;
        col = (image->width - 1) / 7;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = (image->height - 1) / 5;
        col = (image->width - 1) / 3;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));

        row = image->height - 1;
        col = image->width - 1;
        printf("Intensity at <%d,%d>: %d\n", row, col, get_image_intensity(image, row, col));
        
        printf("\n");
        delete_image(image);
    }
    else {
        printf("Failed to load image: %s\n", filename);
    }
}

void run_rmse_unit_test(char *filename, short i) {
    Image *image = load_image(filename);
    (void)i;
    if (image) {
        printf("run_rmse_unit_test for %s\n", filename);

        unsigned int row = 0;
        unsigned int col = 0;

        unsigned char rmse = compute_rmse(image, row, col, image->height, image->width);
        printf("RMSE is %hhu\n\n.", rmse);
        delete_image(image);
    }
    else {
        printf("Failed to load image: %s\n", filename);
    }
}

bool run_load_preorder_unit_test(char *filename, int i) {
    char output_file1[128];
    char output_file2[128];

    snprintf(output_file1, sizeof(output_file1), "tests/output/save_preorder_qt1_qtree__unit_test_file-%d_output1.txt", i);
    snprintf(output_file2, sizeof(output_file2), "tests/output/save_preorder_qt1_qtree__unit_test_file-%d_output2.txt", i);

    Image *image = load_image(filename);
    QTNode *root = create_quadtree(image, 25);

    save_preorder_qt(root, output_file1);
    delete_quadtree(root);
    delete_image(image);

    root = load_preorder_qt(output_file1);
    save_preorder_qt(root, output_file2);
    delete_quadtree(root);

    return compare_files(output_file1, output_file2);
}

bool compare_files(const char *file1, const char *file2) {
    FILE *fp1 = fopen(file1, "r");
    if (fp1 == NULL) {
        perror("Error opening first file");
        return false;
    }

    FILE *fp2 = fopen(file2, "r");
    if (fp2 == NULL) {
        perror("Error opening second file");
        fclose(fp1);
        return false;
    }

    int ch1;
    int ch2;
    bool are_equal = true;

    while (true) {
        ch1 = fgetc(fp1);
        ch2 = fgetc(fp2);

        if (ch1 != ch2) {
            are_equal = false;
            break;
        }

        //if both reach EOF then files are identical
        if (ch1 == EOF && ch2 == EOF) {
            break;
        }

        // if one reaches EOF but the other does not then files are different
        if (ch1 == EOF || ch2 == EOF) {
            are_equal = false;
            break;
        }
    }

    fclose(fp1);
    fclose(fp2);

    return are_equal;
}