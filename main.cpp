#include "Image_Class.h"
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;
Image image;
string filename;
vector<Image> v;
vector<Image> v2;
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
    cout << "10) Filter: Frame\n";
    cout << "11) Filter: Edge Detection\n";
    cout << "12) Filter: Resize\n";
    cout << "13) Filter: Blur\n";
    cout << "14) Filter:enhanceSunlight\n";
    cout << "15) Filter: oil painting\n";
    cout << "16) Filter: Den Den Mushi\n";
    cout << "17) Undo\n";
    cout << "18) Redo\n";
    cout << "19) Save current image\n";
    cout << "20) Exit\n";
    cout << "Select option: ";
}
void Grayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg =0;
            for (int k = 0; k < 3; ++k) {
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

void rotate180(Image& image) {
    Image rotated(image.width, image.height);
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < 3; k++) {
                rotated(image.width-1-i, image.height-1-j, k) = image(i,j,k);
            }
        }
    }
    image = rotated;
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
                image(x,y,z)=min(255,val);
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

void Simple_Frame(Image& image, int thickness, unsigned char r, unsigned char g, unsigned char b) {
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            if (i<thickness || j<thickness || i>=image.width-thickness || j>=image.height-thickness) {
                image(i, j, 0)=r;  
                image(i, j, 1)=g;  
                image(i, j, 2)=b;  
            }
        }
    }
}
void Decorative_Frame(Image& image, int thickness, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2) {
    Simple_Frame(image, thickness, r1, g1, b1);
    for (int i=thickness; i<image.width-thickness; i++) {
        for (int j=thickness; j<image.height-thickness; j++) {
            if (i<2*thickness || j<2*thickness || i>=image.width-2*thickness || j>=image.height-2*thickness) {
                image(i, j, 0)=r2;
                image(i, j, 1)=g2;
                image(i, j, 2)=b2;
            }
        }
    }
    int corner=thickness/3;  
    for (int i=0; i<corner; i++) {    
        for (int j=0; j<thickness; j++) {   
            image(i+5, j+5, 0)=255;
            image(i+5, j+5, 1)=255;
            image(i+5, j+5, 2)=255;
            image(j+5, i+5, 0)=255;
            image(j+5, i+5, 1)=255;
            image(j+5, i+5, 2)=255;
            image(image.width-i-6, image.height-j-6, 0)=255;
            image(image.width-i-6, image.height-j-6, 1)=255;
            image(image.width-i-6, image.height-j-6, 2)=255;
            image(image.width-j-6, image.height-i-6, 0)=255;
            image(image.width-j-6, image.height-i-6, 1)=255;
            image(image.width-j-6, image.height-i-6, 2)=255;
        }
    }
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

void Blur_Level1(Image& image) {
    Image temp=image;
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            for (int k=0; k<3; k++) {
                int sum=0;
                int count=0;
                for (int di=-1; di<=1; di++) {
                    for (int dj=-1; dj<=1; dj++) {
                        int ni=i+di; 
                        int nj=j+dj; 
                        if (ni>=0 && ni<image.width && nj>=0 && nj<image.height) {
                            sum+=temp(ni, nj, k);
                            count++;
                        }
                    }
                }
                image(i, j, k)=sum/count;
            }
        }
    }  
}
void Blur_Level2(Image& image) {
    Image temp=image;
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            for (int k=0; k<3; k++) {
                int sum=0;
                int count=0;
                for (int di=-3; di<=3; di++) {
                    for (int dj=-3; dj<=3; dj++) {
                        int ni=i+di; 
                        int nj=j+dj; 
                        if (ni>=0 && ni<image.width && nj>=0 && nj<image.height) {
                            sum+=temp(ni, nj, k);
                            count++;
                        }
                    }
                }
                image(i, j, k)=sum/count;
            }
        }
    }  
}
void Blur_Level3(Image& image) {
    Image temp=image;
    for (int i=0; i<image.width; i++) {
        for (int j=0; j<image.height; j++) {
            for (int k=0; k<3; k++) {
                int sum=0;
                int count=0;
                for (int di=-5; di<=5; di++) {
                    for (int dj=-5; dj<=5; dj++) {
                        int ni=i+di; 
                        int nj=j+dj; 
                        if (ni>=0 && ni<image.width && nj>=0 && nj<image.height) {
                            sum+=temp(ni, nj, k);
                            count++;
                        }
                    }
                }
                image(i, j, k)=sum/count;
            }
        }
    }  
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

void Den_Den_Mushi(Image& image){
    for (int i=0; i<image.width; i++){
        for (int j=0; j<image.height; j++){
            for (int k=0; k<3; k++){
                if (j%3==0){
                    for (int l=0; l<image.width; l++){
                        image(i, j, 0)=0;
                        image(i, j, 1)=0;
                        image(i, j, 2)=0;
                    }
                }
            }
        }
    }
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
    v.push_back(image);
    bool flag = true;
    while (flag) {
        showMenu();
        int option;
        cin >> option;
        image = v.back();
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
                v.push_back(image);
                break;
            case 2:
                Grayscale(image);
                v.push_back(image);
                break;
            case 3:
                Black_and_White(image);
                v.push_back(image);
                break;
            case 4:
                Invert_Image(image);
                v.push_back(image);
                break;
            case 5: {
                cout << "Please enter the second file name: ";
                string secondFile;
                getline(cin, secondFile);
                while (!fileExists(secondFile)) {
                    cout << "File name you entered is not valid\n";
                    cout << "Please enter a valid image name: \n";
                    getline(cin , secondFile);
                }
                Image b(secondFile);
                Image merged;
                int merge_option;
                cout << "Merge options:\n";
                cout << "1. Resize to largest dimensions\n";
                cout << "2. Use common area\n";
                cout << "Enter merge option: ";
                cin >> merge_option;
                mergeImages(image, b, merged, merge_option);
                image = merged;
                v.push_back(image);
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
                v.push_back(image);
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
                v.push_back(image);
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
                v.push_back(image);
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
                v.push_back(image);
                break;
            }
            case 10:
                 cout << "1) Simple Frame\n";
                 cout << "2) Decorative Frame\n";
                 cout << "Select option: ";
                 cin >> option;
                 switch (option) {
                      case 1:
                          cout << "Enter frame thickness: ";
                          int thickness;
                          cin >> thickness;
                          cout << "Enter ratio of the red colour: ";
                          int r;
                          cin >> r;
                          cout << "Enter ratio of the green colour: ";
                          int g;
                          cin >> g;
                          cout << "Enter ratio of the blue colour: ";
                          int b;
                          cin >> b;
                          Simple_Frame(image, thickness, r, g, b);
                          break;
                     case 2:
                          cout << "Enter frame thickness: ";
                          int thickness;
                          cin >> thickness;
                          cout << "Enter ratio of the red colour for the outside frame: ";
                          int r1;
                          cin >> r1;
                          cout << "Enter ratio of the green colour for the outside frame: ";
                          int g1;
                          cin >> g1;
                          cout << "Enter ratio of the blue colour for the outside frame: ";
                          int b1;
                          cin >> b1;
                          cout << "Enter ratio of the red colour for the inside frame: ";
                          int r2;
                          cin >> r2;
                          cout << "Enter ratio of the green colour for the inside frame: ";
                          int g2;
                          cin >> g2;
                          cout << "Enter ratio of the blue colour for the inside frame: ";
                          int b2;
                          cin >> b2;
                          Decorative_Frame(image, thickness, r1, g1, b1, r2, g2, b2);
                          break;
                 }  
                 v.push_back(image);
                 break;
            case 11:{
                Edge_Detection(image);
                v.push_back(image);
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
                v.push_back(image);
                break;
            }
            case 13:
                cout << "1) Level 1 of bluring\n";
                cout << "2) Level 2 of bluring\n";
                cout << "3) Level 3 of bluring\n";
                cout << "Select option: ";
                cin >> option;
                switch (option) {
                      case 1:
                          Blur_Level1(image);
                          break;
                      case 2:
                          Blur_Level2(image);
                          break;
                      case 3:
                          Blur_Level3(image);
                          break;
                }
                v.push_back(image);
                break;
            case 14:{
                enhanceSunlight(image);
                v.push_back(image);
                break;
            }
            case 15:{
                oil_painting(image);
                v.push_back(image);
                break;
            }
            case 16:
                Den_Den_Mushi(image);
                v.push_back(image);
                break;
            case 17: {
                if (v.size() >= 1) {
                    v2.push_back(v.back());
                    v.pop_back();
                }
                else {
                    cout << "There is nothing to undo \n";
                }
                break;
            }
            case 18: {
                if (v2.size() >= 0) {
                    v.push_back(v2.back());
                    v2.pop_back();
                }
                else {
                    cout << "There is nothing to redo \n";
                }
                break;
            }
            case 19:{
                save_image(image);
                break;
            }
            case 20:{
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
