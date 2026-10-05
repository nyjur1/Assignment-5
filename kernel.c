#include "loader.h"
#include <stdlib.h>
#include <string.h>

/** Returns p1 with each channel multiplied by scalar. */
struct pixel mul(struct pixel p1, float scalar) {
    return (struct pixel){r: p1.r * scalar, g: p1.g * scalar, b: p1.b * scalar};
}
/** Returns the channel-wise sum of p1 and p2. */
struct pixel add(struct pixel p1, struct pixel p2) {
    return (struct pixel){r: p1.r + p2.r, g: p1.g + p2.g, b: p1.b + p2.b};
}

/**
 * Applies a square kernel to an image (cross-correlation).
 *
 * Produces a new image where each output pixel is the weighted sum of
 * the ksize x ksize neighborhood centered on the corresponding input
 * pixel, multiplied by normalize. The kernel is applied as-is (not
 * flipped), so this is technically cross-correlation; the result is
 * identical to convolution for symmetric kernels.
 *
 * The input img is padded so that kernel operations that fall outside of the 
 * original image are multiplied by a black pixel (zero padding).
 *
 * img        Source image. Not modified.
 * kernel     Kernel weights in row-major order, containing ksize * ksize elements.
 * ksize      Width and height of the kernel. Should be odd
 * normalize  Scale factor applied to each weighted sum
 *                       (e.g., 1.0f / 9 for a 3x3 box blur).
 *
 * Returns a pointer to a newly allocated image with the same dimensions as img.
 *
 */
struct image* apply_kernel(struct image* img, int* kernel, int ksize, float normalize) {
    struct image* output = malloc(sizeof(struct image));
    output->width = img->width;
    output->height = img->height;
    output->pixels = malloc(sizeof(struct pixel) * img->width* img->height);

    int center = ksize/2;

    for(int row = 0; row<img->height; row++){
        for(int col = 0; col<img->width; col++){
            struct pixel total = {0,0,0};

            for(int krow= 0; krow<ksize; krow++){
                for(int kcol=0; kcol<ksize; kcol++){

                    int image_row = row + krow - center;
                    int image_col = col + kcol - center;

                    struct pixel cur_pixel = {0,0,0};

                    if(image_row>=0 && image_row<img->height && image_col>=0 && image_col<img->width){

                        int img_index = image_row * img->width + image_col;
                        cur_pixel = img->pixels[img_index];

                    }

                        int kernel_index = krow*ksize+kcol;

                        int kernel_num = kernel[kernel_index];

                        struct pixel product = mul(cur_pixel, kernel_num);

                        total = add(total, product);

                    }

                }
            
            
            total = mul(total, normalize);
            int output_index = row*output->width+col;
            output->pixels[output_index]=total;

        
        }
    }

    return output; 
}

    

