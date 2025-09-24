#include "Image_Class.h"
using namespace std;
Image image;

void Grayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg =0;
            for (int k = 0; k < std::min(image.channels, 3); ++k) {
                avg+= image(i,j,k);
            }
            avg /= 3;
            for (int k=0;k<3;++k) {
                image(i,j,k)=avg;
            }
        }
    }
}
void Black_and_White(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k);
            }
            if (avg < 128) {
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }
            else {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
        }
    }
}
void Invert_Image(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = 255-image(i, j, k);
            }
        }
    }
}

void mergeImages(const Image& a, const Image& b, Image& merged, int merge_option) {
    if (merge_option == 1) {
        int max_w = (a.width > b.width) ? a.width : b.width;
        int max_h = (a.height > b.height) ? a.height : b.height;
        merged = Image(max_w, max_h);
        for (int x = 0; x < max_w; ++x) {
            for (int y = 0; y < max_h; ++y) {
                int a_x = (x * a.width) / max_w;
                int a_y = (y * a.height) / max_h;
                int b_x = (x * b.width) / max_w;
                int b_y = (y * b.height) / max_h;
                for (int c = 0; c < 3; ++c) {
                    merged(x, y, c) = (a(a_x, a_y, c) + b(b_x, b_y, c)) / 2;
                }
            }
        }
    } else {
        int w = (a.width < b.width) ? a.width : b.width;
        int h = (a.height < b.height) ? a.height : b.height;
        merged = Image(w, h);
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                for (int c = 0; c < 3; ++c) {
                    merged(x, y, c) = (a(x, y, c) + b(x, y, c)) / 2;
                }
            }
        }
    }
}
void Horizontal_Flip(Image &image) {
    for (int i = 0; i < image.width/2; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                int temp = image(i, j, k);
                image(i, j, k) = image(image.width-i-1, j, k);
                image(image.width-i-1, j, k) = temp;
            }
        }
    }
}
void Vertical_Flip(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height/2; ++j) {
            for (int k = 0; k < 3; ++k) {
                int temp = image(i, j, k);
                image(i, j, k) = image(i, image.height-j-1, k);
                image(i, image.height-j-1, k) = temp;
            }
        }
    }
}
void rotate180(Image& image){
    for (int i=0; i<image.width; i++){
        for (int j=0; j<image.height/2; j++){
            for (int k=0; k<3; k++){
                int temp=image(i,j,k);
                image(i,j,k)=image(i, image.height-1-j, k);
                image(i, image.height-1-j, k)=temp;
            }
         }
    } 
}

void rotate270(Image& image){
    Image rotated(image.height, image.width);
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            for (int k=0; k<3; k++){
                rotated(j, image.width-1-i, 0)=image(i,j,0);
                rotated(j, image.width-1-i, 1)=image(i,j,1); 
                rotated(j, image.width-1-i, 2)=image(i,j,2);
            }     
        }
    }
    image=rotated;
}

void rotate90(Image& image){
    Image rotated(image.height, image.width);
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            for (int k=0; k<3; k++) {
                rotated(image.height-1-j, i, k)=image(i,j,k);
            }
        }
    }
    image=rotated;
}


