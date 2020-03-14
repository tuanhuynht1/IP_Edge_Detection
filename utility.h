#ifndef UTILITY_H
#define UTILITY_H

#include "image.h"
#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>

enum mask_type {SOBEL, SOBEL_J , SOBEL_I, SOBEL5, SOBEL5_J, SOBEL5_I};


class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		
		//parse helper function
		static vector<char*> parse(char* str, int argc);


		//get mask 
		static vector<vector<int>> getMask(mask_type m);

		//apply mask
		static vector<vector<int>> applyMask(mask_type mask, image& src, Region roi);

		//amplitude detection 
		static void edgeDetection(image& src, image& tgt, mask_type m, int threshold, Region roi);

		//direction detection
		static void directionDectection(image& src, image& tgt, mask_type m, float degree, Region roi);

		//RGB to greyscale
		static void splitRGB(image& src, image& red, image& green, image& blue);

		//combine back to PPM
		static void mergePPM(image& ppmImg, image& binImg, image& tgt, Region roi);

		//combine separate channel edges
		static void combineRGBEdge(image& r, image& g, image& b, image& tgt);


};



#endif

