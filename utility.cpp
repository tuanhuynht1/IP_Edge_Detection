#include "utility.h"

#define MAXRGB 255
#define MINRGB 0
#define HISTO_SIZE 256

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

//----------------------Helper Parse Function ----------------//
vector<char*> utility::parse(char* str, int argC){
	vector<char*> arguments;
	char *pch;
	pch = strtok(str, " ");
	arguments.push_back(pch);
	for(int i = 1; i < argC; i++){
		pch = strtok(NULL, " ");
		arguments.push_back(pch);
	}
	return arguments;
}

vector<vector<int>> utility::getMask(mask_type m){
	switch(m){
		case SOBEL_I:
			return 
			{{1,2,1},
			{0,0,0},
			{-1,-2,-1}};
		case SOBEL_J:
			return
			{{-1,0,1},
			{-2,0,2},
			{-1,0,1}};
	}
}

vector<vector<int>> utility::applyMask(mask_type m, image& src, Region roi){
	//get mask to apply
	vector<vector<int>> M = getMask(m);
	vector<vector<int>> delta(roi.ilen, vector<int>(roi.jlen,0));
	//mask size
	int size = M.size();
	//offset from midpoint
	int offset = size/2;



	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;



	for(int i = roi.i0 + offset; i < roi.ilim - offset; i++){
		for (int j = roi.j0 + offset; j < roi.jlim - offset; j++){
			
			int sum = 0;

			//mask incrementor
			int mask_i = 0, mask_j = 0;
			for(int local_i = i - offset; local_i < i - offset + size; local_i++){
				for(int local_j = j - offset; local_j < j - offset + size; local_j++){
					sum += (M[mask_i][mask_j] * src.getPixel(local_i,local_j));
					mask_j = (mask_j + 1) % size;
				}
				mask_i = (mask_i + 1) % size;
			}

			delta[i-roi.i0][j-roi.j0] = sum;
			// cout << sum << " ";

		}
		// cout << endl;
	}
	// cout << "done ";
	return delta; 

}

void utility::sobelEdgeDetection(image& src, image& tgt, int threshold, Region roi){
	tgt.copyImage(src);
	vector<vector<int>> iDelta = applyMask(SOBEL_I,src,roi);

	for(int i = 0; i < iDelta.size(); i++){
		for(int j = 0; j < iDelta[0].size(); j++){
			cout << iDelta[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	vector<vector<int>> jDelta = applyMask(SOBEL_J,src,roi);

	for(int i = 0; i < jDelta.size(); i++){
		for(int j = 0; j < jDelta[0].size(); j++){
			cout << jDelta[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

}
