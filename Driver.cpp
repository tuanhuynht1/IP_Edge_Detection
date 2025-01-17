/************************************************************
*	  Edge Dectection IPTOOLS							    *
*		Author: Tuan Huynh									*
 ************************************************************/

#include "utility.h"
#include "image.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
#define MAXLEN 256

int main (int argc, char** argv){

	image src;
	FILE *fp;
	char str[MAXLEN];

	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);

		// cout << "TESTING\n" << endl;
		image src, tgt1, tgt2, tgt3, tgt4, h, s, i;
		src.read(strdup("wheel.pgm"));
		Region R(0,0,src.getNumberOfRows(),src.getNumberOfColumns());

		// // vector<double> hsi = image::RGBtoHSI(200,100,30);
		// // cout << hsi[HUE] << " " << hsi[SATURATION] << " " << hsi[INTENSITY] << endl;
		// // vector<double> rgb = image::HSItoRGB(hsi[HUE],hsi[SATURATION],hsi[INTENSITY]);
		// // cout << rgb[RED] << " " << rgb[GREEN] << " " << rgb[BLUE] << endl;

		utility::thresholdDetection(src,tgt1,SOBEL5,40,R);
		tgt1.save("test.pgm");
		utility::directionDectection(tgt1,tgt2,SOBEL5,45,R);
		tgt2.save("dir.pgm");
	
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
					auto started = chrono::high_resolution_clock::now();
					utility::amplitudeDetection(src,tgt,msk,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << "Grey-scale Amplitude Edge " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
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
					auto started = chrono::high_resolution_clock::now(); 
					utility::thresholdDetection(src,tgt,msk,threshold,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << "Grey-scale Threshold Edge " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
					src.copyImage(tgt); //update source for next roi  

				}
			}					
			name = name + "_threshold.pgm";
			tgt.save(name.c_str());
		}

		//Greyscale Direction Detection//---------------------------------------------------------------------
		else if(op.compare("GSCD") == 0){
			image bin,tgt;
			string mask_code;
			int threshold;
			int degree;
			for(int i = 0; i < number_of_regions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){

					//read in arguments 
					argV = utility::parse(str,7);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					mask_code = argV[4];
					threshold = atoi(argV[5]);
					degree = atoi(argV[6]);

					//set up variables and data structures
					Region roi(i_origin,j_origin,rows,cols);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//perform thresholding first
					auto started = chrono::high_resolution_clock::now();
					utility::thresholdDetection(src,bin,msk,threshold,roi);
					utility::directionDectection(bin,tgt,msk,degree,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << "Grey-scale Edge Direction " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
					src.copyImage(tgt); //update source for next roi  

				}
			}					
			name = name + "_direction.pgm";
			tgt.save(name.c_str());
		}
		
		//RGB threshold Detection//---------------------------------------------------------------------
		else if(op.compare("RGBT") == 0){
			image r, g, b, tgt, tgtRGB;
			vector<Region> R;
			string mask_code, filename;
			int threshold;

			//split color image into 3 separate channels first
			auto start = chrono::high_resolution_clock::now();
			utility::splitRGB(src,r,g,b);
			auto fin = chrono::high_resolution_clock::now();
			cout << "Split RGB Channels " << "(" << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "): ";
			cout << chrono::duration_cast<chrono::milliseconds>(fin-start).count() << " ms" << endl;
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
					R.push_back(roi);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//red channel
					auto started = chrono::high_resolution_clock::now();
					utility::thresholdDetection(r,tgt,msk,threshold,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << "Red Channel Edge " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
					r.copyImage(tgt); //update red source for next roi
					
					//green channel
					started = chrono::high_resolution_clock::now();
					utility::thresholdDetection(g,tgt,msk,threshold,roi);
					done = chrono::high_resolution_clock::now();
					cout << "Green Channel Edge " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
					g.copyImage(tgt); //update green source for next roi
					
					//blue channel
					started = chrono::high_resolution_clock::now();
					utility::thresholdDetection(b,tgt,msk,threshold,roi);
					done = chrono::high_resolution_clock::now();
					cout << "Blue Channel Edge " << "(" << roi.ilen << "x" << roi.jlen << "): ";
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
					b.copyImage(tgt); //update blue source for next roi 

				}
			}
	
			filename = name + "_threshold_R.pgm";
			r.save(filename.c_str());
			filename = name + "_threshold_G.pgm";
			g.save(filename.c_str());
			filename = name + "_threshold_B.pgm";
			b.save(filename.c_str());

			//combine RGB by ORing and merge with source image
			auto started = chrono::high_resolution_clock::now();
			utility::combineRGBEdge(r,g,b,tgtRGB,R);
			utility::mergePPM(src,tgtRGB,tgt,R);
			auto done = chrono::high_resolution_clock::now();
			cout << "Combine Edges (" << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "): ";
			cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
			
			filename = name + "_threshold_RGB.ppm";
			tgt.save(filename.c_str());
		}

		//HSI Amplitude Detection//---------------------------------------------------------------------
		else if(op.compare("HSIA") == 0){
			image tgt, H, S, I, *ip;
			vector<Region> R;
			string mask_code, seq = "";
			char hsi;
			//split source into hsi channels first
			auto started = chrono::high_resolution_clock::now();
			utility::splitHSI(src,H,S,I);
			auto done = chrono::high_resolution_clock::now();
			cout << "Convert RGB to HSI (" << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "): ";
			cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

			started = chrono::high_resolution_clock::now();
			for(int i = 0; i < number_of_regions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){

					//read in arguments 
					argV = utility::parse(str,6);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					mask_code = argV[4];
					hsi = argV[5][0];

					//set up variables and data structures
					Region roi(i_origin,j_origin,rows,cols);
					R.push_back(roi);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//perform operation on specific channel
					switch(hsi){
						case 'H':
							ip = &H;
							seq += 'H'; 
							break;
						case 'S':
							ip = &S; 
							seq += 'S';
							break;
						case 'I':
							ip = &I;
							seq += 'I'; 
							break;
					}
					utility::amplitudeDetection(*ip,tgt,msk,roi);
					ip->copyImage(tgt); //update temp for next roi  
				}
			}

			for(int i = 0; i < seq.size(); i++){
				switch(seq[i]){
					case 'H':
						utility::mergePPM(src,H,tgt,{R[i]});
						break;
					case 'S':
						utility::mergePPM(src,S,tgt,{R[i]});
						break;
					case 'I':
						utility::mergePPM(src,I,tgt,{R[i]});
						break;
				}
				src.copyImage(tgt); //update for next roi
			}
			done = chrono::high_resolution_clock::now();
			cout << "HSI Amplitude Edge";
			for(auto r : R){
				cout << " (" << r.ilen << "x" << r.jlen << ")";
			}
			cout << ": " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;
			name = name + "_amplitude_HSI.ppm";
			tgt.save(name.c_str());
		}

		//HSI Threshold Detection//---------------------------------------------------------------------
		else if(op.compare("HSIT") == 0){
			image tgt, H, S, I, *ip;
			vector<Region> R;
			string mask_code, seq = "";
			char hsi;
			int threshold;

			//split source into hsi channels first
			auto started = chrono::high_resolution_clock::now();
			utility::splitHSI(src,H,S,I);
			auto done = chrono::high_resolution_clock::now();
			cout << "Convert RGB to HSI (" << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "): ";
			cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

			started = chrono::high_resolution_clock::now();
			for(int i = 0; i < number_of_regions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){

					//read in arguments 
					argV = utility::parse(str,7);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					mask_code = argV[4];
					threshold = atoi(argV[5]);
					hsi = argV[6][0];

					//set up variables and data structures
					Region roi(i_origin,j_origin,rows,cols);
					R.push_back(roi);
					mask_type msk;
					if(mask_code.compare(0,2,"S3") == 0) {msk = SOBEL;}
					else if(mask_code.compare(0,2,"S5") == 0) {msk = SOBEL5;}
					else{
						cout << "Invalid mask code. Valid options are [S3] and [S5]" << endl;
						exit(1);
					}

					//perform operation on specific channel
					switch(hsi){
						case 'H':
							ip = &H;
							seq += 'H'; 
							break;
						case 'S':
							ip = &S; 
							seq += 'S';
							break;
						case 'I':
							ip = &I;
							seq += 'I'; 
							break;
					}
					utility::thresholdDetection(*ip,tgt,msk,threshold,roi);
					ip->copyImage(tgt); //update temp for next roi  
				}
			}

			for(int i = 0; i < seq.size(); i++){
				switch(seq[i]){
					case 'H':
						utility::mergePPM(src,H,tgt,{R[i]});
						break;
					case 'S':
						utility::mergePPM(src,S,tgt,{R[i]});
						break;
					case 'I':
						utility::mergePPM(src,I,tgt,{R[i]});
						break;
				}
				src.copyImage(tgt); //update for next roi
			}
			done = chrono::high_resolution_clock::now();
			cout << "HSI Threshold Edge";
			for(auto r : R){
				cout << " (" << r.ilen << "x" << r.jlen << ")";
			}
			cout << ": " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

			name = name + "_threshold_HSI.ppm";
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