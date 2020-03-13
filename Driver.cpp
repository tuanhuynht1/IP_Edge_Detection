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
		Region R(50,50,350,350);
		image src, tgt1, tgt2, r, g, b;
		src.read(strdup("frog.ppm"));

		utility::splitRGB(src,r,g,b);
		r.save("red.pgm");
		g.save("green.pgm");
		b.save("blue.pgm");
		

		// utility::edgeDetection(src,tgt1,SOBEL,300,R);
		// tgt1.save("test_5.pgm");

		// utility::directionDectection(tgt1,tgt2,SOBEL,-135,R);
		// tgt2.save("test_5_dir.pgm");

		// utility::directionDectection(tgt1,tgt2,SOBEL,45,R);
		// tgt2.save("test2.pgm");

		// vector<vector<int>> delta = utility::applyMask(SOBEL_I,src,R);
		// for(int i = 0; i < delta.size(); i++){
		// 	for(int j = 0; j < delta[0].size(); j++){
		// 		cout << delta[i][j] << " ";
		// 	}
		// 	cout << endl;
		// }








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