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
void Invert_Image(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = 255-image(i, j, k);
            }
        }
    }
}
void Invert_Image(Image& image){
    for (int i=0; i<image.width; i++){
        for (int j=0; j<image.height; j++){
            for (int k=0; k<image.channels; k++){
                image(i,j,k)=255-image(i,j,k);
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


