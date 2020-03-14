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
		
		//parsing tools
		static vector<char*> parse(char* str, int argc);

		//masking tools
		static vector<vector<int>> getMask(mask_type m);
		static vector<vector<int>> applyMask(mask_type mask, image& src, Region roi);

		//greyscale edge detection tools
		static void amplitudeDetection(image& src, image& tgt, mask_type m, Region roi);
		static void thresholdDetection(image& src, image& tgt, mask_type m, int threshold, Region roi);
		static void directionDectection(image& src, image& tgt, mask_type m, float degree, Region roi);

		//color image edge detection tools
		static void splitRGB(image& src, image& red, image& green, image& blue);
		static void combineRGBEdge(image& r, image& g, image& b, image& tgt, Region roi);
		static void mergePPM(image& ppmImg, image& binImg, image& tgt, Region roi);


};



#endif

