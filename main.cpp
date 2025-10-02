
#include "Image_Class.h"
#include <fstream>
#include <algorithm>

using namespace std;
Image image;
string filename;

bool fileExists(const string &name) {
    ifstream f(name);
    return f.good();
}
bool validExt(const string &name) {
    string s = name;
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s.size() >= 5 && s.rfind(".jpeg") == s.size()-5) return true;
    if (s.size() >= 4 && (s.rfind(".jpg") == s.size()-4 ||
                          s.rfind(".png") == s.size()-4 ||
                          s.rfind(".bmp") == s.size()-4)) return true;
    return false;
}

void showMenu() {
    cout << "\n--- Image Menu ---\n";
    cout << "1) Load a new image\n";
    cout << "2) Filter: Grayscale\n";
    cout << "3) Filter: Black and White\n";
    cout << "4) Filter: Invert Image\n";
    cout << "5) Filter: Merge with another image\n";
    cout << "6) Filter: Flip\n";
    cout << "7) Filter: Rotate\n";
    cout << "8) Filter: Darken / Lighten\n"; 
    cout << "9) Save current image\n";
    cout << "10) Exit\n";
    cout << "Select option: ";
}
void Grayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg =0;
            for (int k = 0; k < min(image.channels, 3); ++k) {
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
    long long total = 0;
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int luminance = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
            total += luminance;
        }
    }
    unsigned int threshold = total / (image.width * image.height);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int luminance = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);

            unsigned char value = (luminance < threshold) ? 0 : 255;
            image(i, j, 0) = value;
            image(i, j, 1) = value;
            image(i, j, 2) = value;
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

void Darken_Image(Image &image, float percent) {
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            for (int z = 0; z < 3; z++) {
                image(x, y, z) = image(x, y, z) * (1 - percent);
            }
        }
    }
}

void Lighten_Image(Image &image, float percent) {
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            for (int z = 0; z < 3; z++) {
                image(x, y, z) = image(x, y, z) + (255 - image(x, y, z)) * percent;
            }
        }
    }
}

void crop(int w, int h, int w1, int h1, Image & image) {
    Image cropped(w, h);
    for (int i=0; i<w; i++) {
        for (int j=0; j<h; j++) {
            for (int k=0; k<3; k++) {
                cropped(i, j, k)=image(i+w1, j+h1, k);
            }
        }
    }
    image = cropped;
}

void resize(Image &image, int newW, int newH) {
    int oldH = image.height;
    int oldW = image.width;
    float xScale = (float)oldW / newW;
    float yScale = (float)oldH / newH;
    Image output(newW, newH);

    for (int y = 0; y < newH; y++) {
        for (int x = 0; x < newW; x++) {
            int origX = (int)(x * xScale + 0.5f);
            int origY = (int)(y * yScale + 0.5f);
            if (origX >= oldW) origX = oldW - 1;
            if (origY >= oldH) origY = oldH - 1;
            for (int c = 0; c < 3; c++) {
                output(x, y, c) = image(origX, origY, c);
            }
        }
    }
    image = output;
}

void save_image(Image&image) {
    cout << "if you want to save the image in the same file type y or Y, otherwise type any other character: ";
    char x;
    cin >> x;
    if (x == 'Y' || x == 'y') {
        image.saveImage(filename);
    }
    else {
        string filename2;
        cout << "Please enter image name to store new image\n";
        cout << "and specify extension .JPG, .JPEG, .BMP, .PNG: ";
        cin >> filename2;
        while (!validExt(filename2) ){
            cout << "File name you entered is not valid\n";
            cout << "Please enter a valid image name: ";
            cin >> filename2;
        }
        image.saveImage(filename2);
    }
}
int main()
{
    cout << "Please enter the file name: ";
    cin >> filename;
    while (!fileExists(filename)) {
        cout << "File name you entered is not valid\n";
        cout << "Please enter a valid image name: ";
        cin >> filename;
    }
    image.loadNewImage(filename);
    bool flag = true;
    while (flag) {
        showMenu();
        int option;
        cin >> option;
        switch (option) {
            case 1:
                cout << "if you want to save the image before loading new one type y or Y, otherwise type any other character: ";
                char x;
                cin >> x;
                if (x=='y'||x == 'Y') {
                    save_image(image);
                }
                flag = false;
                cout << "Please enter the file name: ";
                cin >> filename;
                while (!image.loadNewImage(filename)) {
                    cout << "File name you entered is not valid";
                    cout << "Please enter a valid image name: ";
                    cin >> filename;
                }
                break;
            case 2:
                Grayscale(image);
                break;
            case 3:
                Black_and_White(image);
                break;
            case 4:
                Invert_Image(image);
                break;
            case 5: {
                cout << "Please enter the second file name: ";
                string secondFile;
                cin >> secondFile;
                Image a(filename);
                Image b(secondFile);
                Image merged;
                int merge_option;
                cout << "Merge options:\n";
                cout << "1. Resize to largest dimensions\n";
                cout << "2. Use common area\n";
                cout << "Enter merge option: ";
                cin >> merge_option;
                mergeImages(a, b, merged, merge_option);
                image = merged;
                break;
            }
            case 6:
                cout << "1) Horizontal Flip\n";
                cout << "2) Vertical Flip\n";
                cout << "Select option: ";
                cin >> option;
                switch (option) {
                    case 1:
                        Horizontal_Flip(image);
                        break;
                    case 2:
                        Vertical_Flip(image);
                        break;
                }
                break;
            case 7:
                cout << "1) Rotate 90 degrees\n";
                cout << "2) Rotate 180 degrees\n";
                cout << "3) Rotate 270 degrees\n";
                cout << "Select option: ";
                cin >> option;
                switch (option) {
                    case 1:
                        rotate90(image);
                    break;
                    case 2:
                        rotate180(image);
                    break;
                    case 3:
                        rotate270(image);
                        break;
                }
                break;
            case 8: {
                int choice;
                cout << "1) Darken\n";
                cout << "2) Lighten\n";
                cout << "Select option: ";
                cin >> choice;
                float percent;
                cout << "Enter percentage (0 to 100): ";
                cin >> percent;
                percent /= 100.0;
               if (choice == 1) {
                   Darken_Image(image, percent);
               } else if (choice == 2) {
                    Lighten_Image(image, percent);
               }
               break;
               }
            case 9:
                save_image(image);
                break;
            case 10:
                cout << "if you want to save the image before exit type y or Y, otherwise type any other character: ";
                char y;
                cin >> y;
                if (y=='y'||y == 'Y') {
                    save_image(image);
                }
                flag = false;
                break;
        }
    }
    return 0;
}
