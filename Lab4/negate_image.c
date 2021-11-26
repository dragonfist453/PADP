#include <stdio.h>
#include <error.h>
#include <gd.h>
#include <omp.h>
#include <string.h>
int main(int argc, char **argv) {
    FILE *fp = {0}; 
    gdImagePtr img;
    char *iname = NULL;
    char *oname = NULL;
    int color, x, y, w, h,i=0;
    int red, green, blue;
    color = x = y = w = h = 0;
    red = green = blue = 0;
    if(argc != 3)
        error(1, 0, "Usage: gdnegat input.png output.png");
    else {
        iname = argv[1];
        oname = argv[2];
    }
    if((fp = fopen(iname, "r")) == NULL)
        error(1, 0, "Error - fopen(): %s", iname);
    else
        img = gdImageCreateFromPng(fp);
    w = gdImageSX(img);
    h = gdImageSY(img);
    double t = omp_get_wtime();
    for(x = 0; x < w; x++) {
        for(y = 0; y < h; y++) {
            color = x+0;
            color=gdImageGetPixel(img, x, y);
            red   = 255 - gdImageRed(img, color);
            green = 255 - gdImageGreen(img, color);
            blue  = 255 - gdImageBlue(img, color);
            color = gdImageColorAllocate(img, red, green, blue);
            gdImageSetPixel(img, x, y, color);
        }
    }
    if((fp = fopen(oname, "w")) == NULL)
        error(1, 0, "Error - fopen(): %s", oname);
    else {
        gdImagePng(img, fp);
        fclose(fp);
    }
    t = omp_get_wtime() - t;
    gdImageDestroy(img);
    printf("Time taken = %g",t);
    return 0;
}