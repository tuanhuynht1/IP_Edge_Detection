#include "statistics.h"

#define MAXRGB 255
#define MINRGB 0
#define HISTO_SIZE 256

Image_Statistics::Image_Statistics(image* src, Region roi){
    //initialize region and image for this statistics object
    img = src; 
    region = roi;
    //resize region if out of bounds of image
    if (region.ilim > img->getNumberOfRows()){
        region.ilim = img->getNumberOfRows();
        region.ilen = region.ilim - region.i0;
    }
    if (region.jlim > img->getNumberOfColumns()){
        region.jlim = img->getNumberOfColumns();
        region.jlen = region.jlim - region.j0;
    }

    //initialize size of pixel map based on size of region
    vector<vector<int>> temp1(region.ilen, vector<int>(region.jlen));
    pixel_map = temp1;

    //initialize histogram with zeros
    vector<int> temp2(HISTO_SIZE,0);   // 0 - 255 
    histogram = temp2;

    //iterate through each pixel in the roi of src image and populate map by offset to region origin
    for(int i = 0; i < region.ilen; i++){
        for(int j = 0; j < region.jlen; j++){
            pixel_map[i][j] = img->getPixel(i + region.i0, j + region.j0);
            int value = pixel_map[i][j];
            histogram[value]++; //increment frequency of that value
        }
    }
    // generateNewHistogram();
    setMode(); //set value with highest frequency
    setMean();
    // cout << mean << endl;
}

void Image_Statistics::setMode(){
    //iterate through histogram to find mode
    int highest = histogram[0];
    mode = 0;
    for(int i = 1; i < HISTO_SIZE; i++){
        if(histogram[i] > highest){
            highest = histogram[i];
            mode = i;
        }
    }
}

void Image_Statistics::setMean(){
    int sum = 0, count = 0;
    for(int i = 0; i < histogram.size(); i++){
        sum += (i*histogram[i]);
        count += histogram[i];
    }
    mean = double(sum) / count;
}

void Image_Statistics::setBOmeans(double& background, double& object, double threshold){
    double sum;
    int count;
    int breakpoint = nearbyint(threshold);  //separates background pixels from object pixels
    // calculate mean for background
    sum = 0;
    count = 0;
    for(int i = 0; i < breakpoint; i++){
        sum += (i*histogram[i]);
        count += histogram[i]; 
    }
    background = sum / count;
    //calculate mean for object
    sum = 0;
    count = 0;
    for(int i = breakpoint; i < histogram.size(); i++){
        sum += (i*histogram[i]);
        count += histogram[i]; 
    }
    object = sum / count;
}

void Image_Statistics::generateNewHistogram(){
    vector<int> empty(HISTO_SIZE,0);   //clear histogram
    histogram = empty;
    for(int i = 0; i < region.ilen; i++){
        for(int j = 0; j < region.jlen; j++){
            histogram[pixel_map[i][j]]++;
        }
    }
}

int Image_Statistics::pixel(int i, int j){
    if ( i >= region.ilim || j >= region.jlim || i < region.i0 || j < region.j0){
        return -1;  //out of bounds
    }
    return pixel_map[i - region.i0][j - region.j0];
}

int Image_Statistics::getMin(){
    for (int i = 1; i < HISTO_SIZE; i++){
        if (histogram[i] > 0){
            return i;
        }
    }
    return MINRGB;
}

int Image_Statistics::getMax(){
    for (int i = MAXRGB - 1; i >= 1; i--){
        if (histogram[i] > 0){
            return i;
        }
    }
    return MINRGB;
}

void Image_Statistics::writeHistogramToFile(string filename){
    image output(HISTO_SIZE,HISTO_SIZE);

    //coefficient for normalizing
    float k = float(HISTO_SIZE - 1) / histogram[mode];

    for(int j = 0; j < HISTO_SIZE; j++){
        int count = int(histogram[j] * k);  //normalize to fit
        for(int i  = HISTO_SIZE - 1; i > HISTO_SIZE-1 - count; i--){
            output.setPixel(i,j,MAXRGB);    //set each count to appear as a white pixel
        }
    }
    output.save(filename.c_str());
}

Color_Image_Statistics::Color_Image_Statistics(image* src, Region roi, channel RGB){
    //initialize region and image for this statistics object
    img = src; 
    region = roi;
    color = RGB;
    //resize region if out of bounds of image
    if (region.ilim > img->getNumberOfRows()){
        region.ilim = img->getNumberOfRows();
        region.ilen = region.ilim - region.i0;
    }
    if (region.jlim > img->getNumberOfColumns()){
        region.jlim = img->getNumberOfColumns();
        region.jlen = region.jlim - region.j0;
    }

    //initialize size of pixel map based on size of region
    vector<vector<int>> temp1(region.ilen, vector<int>(region.jlen));
    pixel_map = temp1;

    //initialize histogram with zeros
    vector<int> temp2(HISTO_SIZE,0);   // 0 - 255 
    histogram = temp2;

    //iterate through each pixel in the roi of src image and populate map by offset to region origin
    for(int i = 0; i < region.ilen; i++){
        for(int j = 0; j < region.jlen; j++){
            pixel_map[i][j] = img->getPixel(i + region.i0, j + region.j0,color);
            int value = pixel_map[i][j];
            histogram[value]++; //increment frequency of that value
        }
    }
    // generateNewHistogram();
    setMode(); //set value with highest frequency
    setMean();
}
