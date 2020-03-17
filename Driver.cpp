/************************************************************
*	  Edge Dectection IPTOOLS							    *
*		Author: Tuan Huynh									*
 ************************************************************/

#include "utility.h"
#include "image.h"
#include "statistics.h"
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

#define MAXLEN 256
// #define M_PI 3.14159


int main (int argc, char** argv){

	image src;
	FILE *fp;
	char str[MAXLEN];

	if ((fp = fopen(argv[1],"r")) == NULL) {
		// fprintf(stderr, "Can't open file: %s\n", argv[1]);
		// exit(1);

		// cout << "TESTING\n" << endl;
		// image src, tgt1, tgt2, tgt3, tgt4, h, s, i;
		// src.read(strdup("004.ppm"));
		// Region R(0,0,src.getNumberOfRows(),src.getNumberOfColumns());

		// // vector<double> hsi = image::RGBtoHSI(200,100,30);
		// // cout << hsi[HUE] << " " << hsi[SATURATION] << " " << hsi[INTENSITY] << endl;
		// // vector<double> rgb = image::HSItoRGB(hsi[HUE],hsi[SATURATION],hsi[INTENSITY]);
		// // cout << rgb[RED] << " " << rgb[GREEN] << " " << rgb[BLUE] << endl;

		// utility::splitHSI(src,h,s,i);
		// h.save("h.pgm");
		// s.save("s.pgm");
		// i.save("i.pgm");	
		// utility::amplitudeDetection(h,tgt1,SOBEL,R);
		// utility::amplitudeDetection(s,tgt2,SOBEL,R);
		// utility::amplitudeDetection(i,tgt3,SOBEL,R);

		// utility::mergePPM(src,tgt1,tgt4,R);	
		// tgt4.save("amp_h.ppm");

		// utility::mergePPM(src,tgt2,tgt4,R);	
		// tgt4.save("amp_s.ppm");

		// utility::mergePPM(src,tgt3,tgt4,R);	
		// tgt4.save("amp_i.ppm");

		
		exit(1);
	}


	vector<char*> argV;
	int rows, cols, i_origin, j_origin, number_of_regions;
	string name, op;

	while(fgets(str,MAXLEN,fp) != NULL){

		//parse line into argument Vector for each operation
		argV = utility::parse(str,4);
		src.read(argV[0]);					//input image file
		name = argV[1];						//name for basis of output files
		op = argV[2];						//operation		
		number_of_regions = atoi(argV[3]);	//number of roi

		//Greyscale Amplitude Detection//---------------------------------------------------------------------
		if(op.compare("GSCA") == 0){
			image tgt;
			string mask_code;
			for(int i = 0; i < number_of_regions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){

					//read in arguments 
					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					mask_code = argV[4];

					//set up variables and data structures
					Region roi(i_origin,j_origin,rows,cols);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//perform operation 
					utility::amplitudeDetection(src,tgt,msk,roi);
					src.copyImage(tgt); //update source for next roi  

				}
			}					
			name = name + "_amplitude.pgm";
			tgt.save(name.c_str());
		}

		//Greyscale Threshold Detection//---------------------------------------------------------------------
		else if(op.compare("GSCT") == 0){
			image tgt;
			string mask_code;
			int threshold;
			for(int i = 0; i < number_of_regions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){

					//read in arguments 
					argV = utility::parse(str,6);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					mask_code = argV[4];
					threshold = atoi(argV[5]);

					//set up variables and data structures
					Region roi(i_origin,j_origin,rows,cols);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//perform operation 
					utility::thresholdDetection(src,tgt,msk,threshold,roi);
					src.copyImage(tgt); //update source for next roi  

				}
			}					
			name = name + "_threshold.pgm";
			tgt.save(name.c_str());
		}



		//Operation not valid //------------------------------------------------------------------------
		else{
			cout << "Invalid opcode \"" << op << "\"" << endl;
		}

	}
	
	fclose(fp);

	return 0;
}