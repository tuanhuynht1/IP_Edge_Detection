#include "utility.h"
#include <cmath>

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
	 
		case SOBEL5_I:
			return
			{{2,2,4,2,2},
			{1,1,2,1,1},
			{0,0,0,0,0,},
			{-1,-1,-2,-1,-1},
			{-2,-2,-4,-2,-2}};
		case SOBEL5_J:
			return
			{{-2,-1,0,1,2},
			{-2,-1,0,1,2},
			{-4,-2,0,2,4},
			{-2,-1,0,1,2},
			{-2,-1,0,1,2}};

		default:
			return {};
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



	// for(int i = 0; i < M.size(); i++){
	// 	for(int j = 0; j < M[0].size(); j++){
	// 		cout << M[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }
	// cout << endl;



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

void utility::edgeDetection(image& src, image& tgt, mask_type m, int threshold, Region roi){
	//copy src image to tgt
	tgt.copyImage(src);

	vector<vector<int>> iDelta;
	vector<vector<int>> jDelta;

	switch (m) {
	case SOBEL:
		//apply verticle mask
		iDelta = applyMask(SOBEL_I,src,roi);

		//apply horizontal mask
		jDelta = applyMask(SOBEL_J,src,roi);
		break;

	case SOBEL5:
		//apply verticle mask
		iDelta = applyMask(SOBEL5_I,src,roi);

		//apply horizontal mask
		jDelta = applyMask(SOBEL5_J,src,roi);
		break;
	
	default:
		return;
	}

	//calculate magnitude
	vector<vector<float>> amplitude(roi.ilen, vector<float>(roi.jlen));
	for(int i = 0; i < amplitude.size(); i++){
		for(int j = 0; j < amplitude[0].size(); j++){
			// sqrt (iDelta^2 + jDelta^2)
			amplitude[i][j] = sqrt((pow(iDelta[i][j],2)+pow(jDelta[i][j],2)));
		}
	}

	//thresholding
	for(int i = roi.i0; i < roi.ilim; i++){
		for(int j = roi.j0; j < roi.jlim; j++){
			if(amplitude[i-roi.i0][j-roi.j0] < threshold){
				tgt.setPixel(i,j,MINRGB);
			}
			else{
				tgt.setPixel(i,j,MAXRGB);
			}
		}
	}
}

void utility::directionDectection(image& src, image& tgt, mask_type m, float degree, Region roi){
	//copy src (should be a binarized image)
	tgt.copyImage(src);

	vector<vector<int>> iDelta;
	vector<vector<int>> jDelta;

	switch (m) {
	case SOBEL:
		//apply verticle mask
		iDelta = applyMask(SOBEL_I,src,roi);

		//apply horizontal mask
		jDelta = applyMask(SOBEL_J,src,roi);
		break;

	case SOBEL5:
		//apply verticle mask
		iDelta = applyMask(SOBEL5_I,src,roi);

		//apply horizontal mask
		jDelta = applyMask(SOBEL5_J,src,roi);
		break;
	
	default:
		return;
	}


	for(int i = 0; i < roi.ilen; i++){
		for(int j = 0; j < roi.jlen; j++){
			// cout << iDelta[i][j] << "\t";
		}
		// cout << endl;
	}
	// cout << endl;
	for(int i = 0; i < roi.ilen; i++){
		for(int j = 0; j < roi.jlen; j++){
			// cout << jDelta[i][j] << "\t";
		}
		// cout << endl;
	}
	// cout << endl;

	//calculate direction
	vector<vector<float>> direction(roi.ilen, vector<float>(roi.jlen));
	for(int i = 0; i < direction.size(); i++){
		for(int j = 0; j < direction[0].size(); j++){
			//arctan2(di/dj) * degree conversion
			direction[i][j] = atan2(iDelta[i][j],jDelta[i][j]) * 180.0/3.14159;
			// cout << direction[i][j] << "\t";
		}
		// cout << endl;
	}
	// cout << endl;

	//thresholding
	for(int i = roi.i0; i < roi.ilim; i++){
		for(int j = roi.j0; j < roi.jlim; j++){
			if( direction[i-roi.i0][j-roi.j0] >= degree - 10 && direction[i-roi.i0][j-roi.j0] <= degree + 10 ){
				tgt.setPixel(i,j,MAXRGB);
				// cout << direction[i-roi.i0][j-roi.j0] << " ";
			}
			else{
				tgt.setPixel(i,j,MINRGB);
			}
		}
	}
}

void utility::splitRGB(image& src, image& red, image& green, image& blue){
	int rows = src.getNumberOfRows(), cols = src.getNumberOfColumns();
	red.resize(rows,cols);
	green.resize(rows,cols);
	blue.resize(rows,cols);

	for(int i = 0 ; i < rows; i++){
		for(int j = 0; j < cols; j++){
			red.setPixel(i,j,src.getPixel(i,j,RED));
			green.setPixel(i,j,src.getPixel(i,j,GREEN));
			blue.setPixel(i,j,src.getPixel(i,j,BLUE));
		}
	}

}

void utility::combineRGBEdge(image& r, image& g, image& b, image& tgt){
	tgt.copyImage(r);
	for(int i = 0; i < tgt.getNumberOfRows(); i++){
		for(int j = 0; j < tgt.getNumberOfColumns(); j++){

			if( 
				(r.getPixel(i,j) == MINRGB || r.getPixel(i,j) == MAXRGB) &&
				(g.getPixel(i,j) == MINRGB || g.getPixel(i,j) == MAXRGB) &&
				(b.getPixel(i,j) == MINRGB || b.getPixel(i,j) == MAXRGB)
			){
				if( r.getPixel(i,j) == MAXRGB ||  g.getPixel(i,j) == MAXRGB ||  b.getPixel(i,j) == MAXRGB){
					tgt.setPixel(i,j,MAXRGB);
				}
				else{
					tgt.setPixel(i,j,MINRGB);
				}
			}

		}
	}
}

void utility::mergePPM(image& ppmImg, image& binImg, image& tgt, Region roi){
	tgt.copyImage(ppmImg);
	for(int i = roi.i0; i < roi.ilim; i++){
		for(int j = roi.j0; j < roi.jlim; j++){
			int val = binImg.getPixel(i,j);
			tgt.setPixel(i,j,RED,val);
			tgt.setPixel(i,j,GREEN,val);
			tgt.setPixel(i,j,BLUE,val);
		}
	}
}



