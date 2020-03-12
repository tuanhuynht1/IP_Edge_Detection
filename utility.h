#ifndef UTILITY_H
#define UTILITY_H

#include "image.h"
#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>

enum mask_type {SOBEL_J= 0, SOBEL_I = 1, SOBEL = 2};


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

		//sobel amplitude 
		static void sobelEdgeDetection(image& src, image& tgt, mask_type m, int threshold, Region roi);

		// static void directionalEdgeDectection(image& src, float degree, Region roi);
};



#endif

