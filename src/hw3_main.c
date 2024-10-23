#include "qtree.h"
#include "image.h"

#include "tests_utils.h"

void run_load_image_unit_test(char *filename);

int main() {
    struct stat st;
    if (stat("tests/output", &st) == -1)
        mkdir("tests/output", 0700);
    prepare_input_image_file("building1.ppm"); // copies the image to the images/ directory
    
    /******************************* prepare all images *******************************/
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


    /******************************* load_image *******************************/
    printf("Start load_image unit test(s)...\n\n");

    for (unsigned short i = 0; i < filenames_len; i++) {
        printf("(%d)\n", i + 1);

        // buffer
        char input_filename[64];
        strcpy(input_filename, "images/");
        strcat(input_filename, filenames[i]);

        run_load_image_unit_test(input_filename);
    }

    printf("End load_image unit test(s)...\n\n");

    /******************************* create_quadtree *******************************/
    double max_rmse = 25;
    Image *image = load_image("images/building1.ppm");
    QTNode *root = create_quadtree(image, max_rmse);
    // See tests/input/load_preorder_qt1_qtree.txt for the expected results
    // You will need to write your own code to verify that your quadtree was constructed properly
    delete_quadtree(root);
    delete_image(image);

    /******************************* load_preorder_qt *******************************/
    // tests/input/load_preorder_qt1_qtree.txt was generated by executing the following code:
    //   QTNode *root = create_quadtree(load_image("images/building1.ppm"), 25);
    root = load_preorder_qt("tests/input/load_preorder_qt1_qtree.txt");
    // You will need to write your own code to verify that your quadtree was constructed properly
    delete_quadtree(root); 

    /******************************* save_preorder_qt *******************************/
    image = load_image("images/building1.ppm"); 
    root = create_quadtree(image, 25);
    // See tests/input/load_preorder_qt1_qtree.txt for expected output
    save_preorder_qt(root, "tests/output/save_preorder_qt1_qtree.txt");
    delete_quadtree(root);
    delete_image(image);

    /******************************* save_qtree_as_ppm *******************************/
    image = load_image("images/building1.ppm"); 
    root = create_quadtree(image, 25);
    save_qtree_as_ppm(root, "tests/output/save_qtree_as_ppm1.ppm");
    // See tests/expected/save_qtree_as_ppm1.ppm for the expected file.
    // Visual inspection is generally not sufficient to determine if your output image is correct.
    // You will need to write code to more rigorously check your output image for correctness.
    delete_image(image); 
    delete_quadtree(root);

    /******************************* hide_message and reveal_message *******************************/
    prepare_input_image_file("wolfie-tiny.ppm");
    hide_message("0000000000111111111122222222223333333333", "images/wolfie-tiny.ppm", "tests/output/hide_message1.ppm");
    char *message = reveal_message("tests/output/hide_message1.ppm");
    printf("Message: %s\n", message);
    free(message);

    /******************************* hide_image and reveal_image *******************************/
    hide_image("images/wolfie-tiny.ppm", "images/building1.ppm", "tests/output/hide_image1.ppm");
    reveal_image("tests/output/hide_image1.ppm", "tests/output/reveal_image1.ppm");

    return 0;
}

void run_load_image_unit_test(char *filename) {
    Image *image1 = load_image(filename);
    if (image1) {
        printf("run_load_image_unit_test for %s\n", filename);
        printf("Width: %d, Height: %d, Intensity: %d\n", image1->width, image1->height, image1->max_intensity);
        printf("Intensity at <0,0>: %d\n", get_image_intensity(image1, 0, 0));
        printf("Intensity at <5,5>: %d\n", get_image_intensity(image1, 5, 5));
        printf("Intensity at <%d,%d>: %d\n", image1->width, image1->height, get_image_intensity(image1, image1->width, image1->height));
        printf("\n");
        delete_image(image1);
    } else {
        printf("Failed to load image: %s\n", filename);
    }
}