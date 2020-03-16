/************************************************************
*	  Edge Dectection IPTOOLS							    *
*		Author: Tuan Huynh									*
 ************************************************************/

#include "utility.h"
#include "image.h"
#include "statistics.h"
#include <vector>
#include <chrono>

using namespace std;

#define MAXLEN 256


int main (int argc, char** argv){

	image src, *ip;
	FILE *fp;
	char str[MAXLEN];

	if ((fp = fopen(argv[1],"r")) == NULL) {
		// fprintf(stderr, "Can't open file: %s\n", argv[1]);
		// exit(1);

		cout << "TESTING\n" << endl;
		Region R(0,150,250,300);
		image src, tgt1, tgt2, tgt3, tgt4, r, g, b;
		src.read(strdup("frog.ppm"));

		// utility::splitRGB(src,r,g,b);
		// utility::thresholdDetection(r,tgt1,SOBEL,40,R);
		// utility::thresholdDetection(g,tgt2,SOBEL,40,R);
		// utility::thresholdDetection(b,tgt3,SOBEL,40,R);

		// utility::combineRGBEdge(tgt1,tgt2,tgt3,tgt4,R);
		// tgt4.save("OR.pgm");
		// utility::mergePPM(src,tgt4,tgt1,R);

		// tgt1.save("comb.ppm");
		
		utility::RGBtoHSI(20,30,102);
	







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

		//debugging//---------------------------------------------------------------------
		if(op.compare(".") == 0){
			cout << "debugging" << endl;
		}
		//Operation not valid //------------------------------------------------------------------------
		else{
			cout << "Invalid opcode \"" << op << "\"" << endl;
		}

	}
	
	fclose(fp);

	return 0;
}