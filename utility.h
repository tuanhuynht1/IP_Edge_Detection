#ifndef UTILITY_H
#define UTILITY_H

#include "image.h"
#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>



class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		
		//HW1 parse helper function
		static vector<char*> parse(char* str, int argc);

};

#endif

