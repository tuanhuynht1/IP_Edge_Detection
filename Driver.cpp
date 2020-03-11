/************************************************************
*	Advanced image and color manipulation					*
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

		exit(1);
	}
	cout << "hi" << endl;
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