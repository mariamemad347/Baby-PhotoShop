#include "Image_Class.h"
#include <fstream>
#include <algorithm>
#include <vector>

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
    cout << "9) Filter: Crop\n";
    cout << "11) Filter: Edge Detection\n";
    cout << "12) Filter: Resize\n";
    cout << "14) Filter:enhanceSunlight\n";
    cout << "15) Filter: oil painting\n";
    cout << "16) Save current image\n";
    cout << "17) Exit\n";
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
            rotated(j, image.width-1-i, 0)=image(i,j,0);
            rotated(j, image.width-1-i, 1)=image(i,j,1);
            rotated(j, image.width-1-i, 2)=image(i,j,2);
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
                int val = image(x, y, z) + (255 - image(x, y, z)) * percent;
                img(x,y,z)=min(255,val);
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

void Edge_Detection(Image &image) {
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            int avg = 0;
            for (int c = 0; c < 3; c++) {
                avg += image(x, y, c);
            }
            avg /= 3;
            for (int c = 0; c < 3; c++) {
                image(x, y, c) = avg;
            }
        }
    }
    int threshold;
    cout << "Enter threshold value (recommended 0 - 100): ";
    cin >> threshold;
    Image edges(image.width, image.height);
    for (int x = 1; x < image.width - 1; x++) {
        for (int y = 1; y < image.height - 1; y++) {
            int left  = image(x - 1, y, 0);
            int right = image(x + 1, y, 0);
            int up    = image(x, y - 1, 0);
            int down  = image(x, y + 1, 0);
            int horizontal = abs(right - left);
            int vertical   = abs(down - up);
            int magnitude  = horizontal + vertical;
            int edgeColor = 255;
            if (magnitude > threshold) {
                edgeColor = 255 - min(magnitude * 2, 255);
            }
            for (int c = 0; c < 3; c++) {
                edges(x, y, c) = edgeColor;
            }
        }
    }
    image = edges;  
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

void enhanceSunlight(Image &image) {
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            // Get original pixel values
            int r = image(x, y, 0);
            int g = image(x, y, 1);
            int b = image(x, y, 2);
            r = min(255, r + 50);
            g = min(255, g + 50);
            b = min(255, b + 50);
            r = min(255, r + 20);
            g = min(255, g + 10);
            b = max(0, b - 15);
            image(x, y, 0) = r;
            image(x, y, 1) = g;
            image(x, y, 2) = b;
        }
    }
}

void oil_painting (Image&image){
    vector<int> intensity_count(256, 0);
    vector<int> averageR(256, 0);
    vector<int> averageG(256, 0);
    vector<int> averageB(256, 0);
    Image final(image.width, image.height);

    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            int r_start = max(0, i - 5);
            int r_end   = min(image.width - 1, i + 5);
            int c_start = max(0, j - 5);
            int c_end   = min(image.height - 1, j + 5);
            for (int r = r_start; r <= r_end; r++) {
                for (int c = c_start; c <= c_end; c++) {
                    int intensity = int(0.299*image(r, c, 0) + 0.587*image(r, c, 1) + 0.114*image(r, c, 2));
                    intensity_count[intensity]++;
                    averageR[intensity] += image(r, c, 0);
                    averageG[intensity] += image(r, c, 1);
                    averageB[intensity] += image(r, c, 2);
                }
            }
            int max = 0;
            int maxindex = 0;
            for (int x = 0; x < intensity_count.size(); x++) {
                if (intensity_count[x] > max) {
                    max = intensity_count[x];
                    maxindex = x;
                }
            }
            if (max>0) {
                final(i, j, 0) = averageR[maxindex]/max;
                final(i, j, 1) = averageG[maxindex]/max;
                final(i, j, 2) = averageB[maxindex]/max;
            }
            else {
                final(i,j,0) = image(i,j,0);
                final(i,j,1) = image(i,j,1);
                final(i,j,2) = image(i,j,2);
            }

            fill(intensity_count.begin(), intensity_count.end(), 0);
            fill(averageR.begin(), averageR.end(), 0);
            fill(averageG.begin(), averageG.end(), 0);
            fill(averageB.begin(), averageB.end(), 0);
        }
    }
    image = final;
}

void save_image(Image&image) {
    cout << "if you want to save the image in the same file type y or Y, otherwise type any other character: ";
    char x;
    cin >> x;
    cin.ignore();
    if (x == 'Y' || x == 'y') {
        image.saveImage(filename);
    }
    else {
        string filename2;
        cout << "Please enter image name to store new image\n";
        cout << "and specify extension .JPG, .JPEG, .BMP, .PNG: \n";
        getline(cin, filename2);
        while (!validExt(filename2) ){
            cout << "File name you entered is not valid\n";
            cout << "Please enter a valid image name: \n";
            getline(cin, filename2);
        }
        image.saveImage(filename2);
    }
}
int main()
{
    cout << "Please enter the file name: \n";
    getline(cin, filename);
    while (!fileExists(filename)) {
        cout << "File name you entered is not valid\n";
        cout << "Please enter a valid image name: \n";
        getline(cin, filename);
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
                cin.ignore();
                if (x=='y'||x == 'Y') {
                    save_image(image);
                }
                cout << "Please enter the file name: \n";
                getline(cin, filename);
                while (!fileExists(filename)) {
                    cout << "File name you entered is not valid\n";
                    cout << "Please enter a valid image name: \n";
                    getline(cin , filename);
                }
                image.loadNewImage(filename);
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
                cin.ignore();
                getline(cin, secondFile);
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
                percent= max(0.0,min(percent/100.0,1.0));
               if (choice == 1) {
                   Darken_Image(image, percent);
               } else if (choice == 2) {
                    Lighten_Image(image, percent);
               }else{
                   cout <<"Invalid choice\n";
               }
               break;
               }
            case 9:{
                cout << "Enter the width of the new image: ";
                int new_width;
                cin >> new_width;
                cout << "Enter the height of the new image: ";
                int new_height;
                cin >> new_height;
                cout << "Enter the width you want to crop from: ";
                int width;
                cin >> width;
                cout << "Enter the height you want to crop from: ";
                int height;
                cin >> height;
                while (new_width+width > image.width || new_height+height > image.height) {
                    cout << "The width and height are out of boundaries\n";
                    cout << "Enter the width of the new image: ";
                    cin >> new_width;
                    cout << "Enter the height of the new image: ";
                    cin >> new_height;
                    cout << "Enter the width you want to crop from: ";
                    cin >> width;
                    cout << "Enter the height you want to crop from: ";
                    cin >> height;
                }
                crop(new_width, new_height,width, height, image);
                break;
            }
            case 11:{
                Edge_Detection(image);
                break;
            }
            case 12:{
                cout << "Enter the width of the new image: ";
                int NewWidth;
                cin >> NewWidth;
                cout << "Enter the height of the new image: ";
                int NewHeight;
                cin >> NewHeight;
                resize(image, NewWidth, NewHeight);
                break;
            }
            case 14:{
                enhanceSunlight(image);
                break;
            }
            case 15:{
                oil_painting(image);
                break;
            }
            case 16:{
                save_image(image);
                break;
            }
            case 17:{
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
    }
    return 0;
}
