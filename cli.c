#include "kernel.h"
#include <string.h>
#include <sys/mman.h>

int generate_pagefault() {

}

int main(int argc, char** argv){
    // TODO: parse the arguments in argv. 
    // You can expect argv[1] to be the mode
    // You can expect argv[2] to be the filepath
    // You can expect argv[3] to be the integer width
    // You can expect argv[4] to be the integer height
    // You can expect argv[5] to be the output filepath.

    if(argc != 6) {
        printf("Incorrect number of arguments. Expected: ./cli <MODE=kernel|mmap|convert|uconvert|fault> <input_image> <width> <height> <output_image_path>\n");
        return -1;

    }

    char* mode = argv[1];
    char* filepath = argv[2];
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    char* output = argv[5];

    // TODO: call correct function based on mode
    if(strcmp(mode, "kernel")==0){
        struct image* img = malloc(sizeof(struct image));
        img->width = width;
        img->height = height;
        loadimage(filepath,img);

        int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

         struct image* final = apply_kernel(img, &kernel[0][0],3 , 1.0f/9.0f);

         saveimage(output,final);
         free(img->pixels);
         free(img);
         free(final->pixels);
         free(final);


    }

     if(strcmp(mode, "convert")==0){
        struct image* img = malloc(sizeof(struct image));
        img->width = width;
        img->height = height;
        loadimage(filepath,img);
        saveimage_mmap(output, img);

        free(img->pixels);
        free(img);
     }

     if(strcmp(mode, "uconvert")==0){
        struct image* img = malloc(sizeof(struct image));
        img->width = width;
        img->height = height;
        loadimage_mmap(filepath,img);
        saveimage(output, img);

        size_t header_size = sizeof(struct image);
	    size_t pixel_size = width * height * sizeof(struct pixel);
	    size_t total_size = (header_size + pixel_size);

        char* mapped = (char*)img->pixels-header_size;
        munmap(mapped, total_size);

        free(img);
     }


      if(strcmp(mode, "mmap")==0){
        struct image* img = malloc(sizeof(struct image));
        img->width = width;
        img->height = height;
        loadimage_mmap(filepath,img);

        int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

        struct image* final = apply_kernel(img, &kernel[0][0],3 , 1.0f/9.0f);

        saveimage(output,final);

        size_t header_size = sizeof(struct image);
	    size_t pixel_size = width * height * sizeof(struct pixel);
	    size_t total_size = (header_size + pixel_size);

        char* mapped = (char*)img->pixels-header_size;
        munmap(mapped, total_size);

        free(img);
        free(final->pixels);
        free(final);

    }

    return 0;






    // TODO: allocate the space needed for one image and load the image


    

    // TODO: call apply kernel with 1/9 (as a float) as the normalization value
   
}
