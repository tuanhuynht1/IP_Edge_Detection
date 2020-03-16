#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>

using namespace std;

enum channel{RED, GREEN, BLUE, GREY=RED, GRAY=GREY,HUE = RED, SATURATION = GREEN, INTENSITY = BLUE};

struct imageData
{
   vector<int>  redChannel, greenChannel, blueChannel; 
   int numRows, numColumns; 
};

struct Region{
	int i0, j0, ilim, jlim, ilen, jlen;
   Region(){};
	Region(int i_origin, int j_origin, int rows, int cols ){
      i0 = i_origin, j0 = j_origin, ilen = rows, jlen = cols;
      ilim = i0 + ilen;
      jlim = j0 + jlen;
   }
};

struct Color{
   int r,g,b;
   Color(int R, int G, int B) : r(R), g(G), b(B) {}
};


class image 
{
private:
   bool HSI = false;
   imageData data;
   int getint(FILE *fp);
   friend class utility;
   
public:
   image();
   image(image &img);
   image(int rows, int columns);
   ~image();
   
   void deleteImage();
   void copyImage(image &img);
   void resize (int numberOfRows, int numberOfColumns);
   void setNumberOfRows(int rows);
   void setNumberOfColumns(int columns);
   void setPixel(const int row, const int col, const int value);
   void setPixel(const int row, const int col, const int rgb, const int value);

   
   int getPixel(const int row, const int col);
   int getPixel(const int row, const int col, const int rgb);
   int getNumberOfRows();
   int getNumberOfColumns(); 
   vector<int>* getChannel(int rgb);
   
   bool setChannel(int rgb, vector<int> &channel);   
   bool save (char* file);
   bool save (const char* file);
   bool read (char* file);
   bool isInbounds (const int row, const int col);


   static vector<double> RGBtoHSI(double R, double G, double B);
   static vector<double> HSItoRGB(double H, double S, double I);
   void convertToHSI();
   void convertToRGB();

};



#endif

