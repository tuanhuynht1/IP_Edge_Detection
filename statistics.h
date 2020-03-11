#ifndef STATISTICS_H
#define STATISTICS_H

#include "image.h"
#include <cmath>

class Image_Statistics{
    public:

    image* img;
    Region region;
    vector<vector<int>> pixel_map;
    vector<int> histogram;
    int mode;
    double mean;

    Image_Statistics(){;}   //default constructor   
    Image_Statistics(image* src, Region roi);
    int pixel(int i, int j);
    int getMin();
    int getMax();
    void setMean();
    void setMode();
    void generateNewHistogram();
    void setBOmeans(double& background, double& object, double threshold);
    void writeHistogramToFile(string filename);

};

//inherits image statistics, enforces a channel specification
class Color_Image_Statistics : public Image_Statistics{
    public:
    channel color;
    Color_Image_Statistics(image* src, Region roi, channel RGB);
};

#endif