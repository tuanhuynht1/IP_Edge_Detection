#include "image.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

image::image()
{
	data.numRows = data.numColumns = 0; 
	data.redChannel.clear();
	data.greenChannel.clear();
	data.blueChannel.clear();
}
/*-----------------------------------------------------------------------**/
image::image(image &img)
{
	this->copyImage(img);
}

/*-----------------------------------------------------------------------**/
image::image(int rows, int columns) 
{
	this->data.numRows = rows;
	this->data.numColumns = columns; 
	this->resize (rows, columns);
}

/*-----------------------------------------------------------------------**/
image::~image() 
{
	this->deleteImage();
}

/*-----------------------------------------------------------------------**/
bool image::isInbounds (int row, int col) 
{
	if ((row < 0) || (col < 0) || (col >= data.numColumns) || (row >= data.numRows))
		return false;
	else 
		return true;
}

/*-----------------------------------------------------------------------**/
void image::deleteImage() 
{
	this->data.numRows = this->data.numColumns = 0; 
	this->data.redChannel.clear();
	this->data.greenChannel.clear();
	this->data.blueChannel.clear();
}

/*-----------------------------------------------------------------------**/
void image::copyImage(image &img) 
{
	this->resize(img.getNumberOfRows() ,img.getNumberOfColumns());

	for (int i = 0; i<3; i++) 
	{
		this->setChannel(i, *img.getChannel(i));
	}
}


/*-----------------------------------------------------------------------**/
void image::resize (int rows, int columns)
{
	int length = rows*columns;
	data.numRows = rows;
	data.numColumns = columns;
	data.redChannel.clear();
	data.greenChannel.clear();
	data.blueChannel.clear();

	data.redChannel.resize(length); 
	data.greenChannel.resize(length);
	data.blueChannel.resize(length);

}

/*-----------------------------------------------------------------------**/
void image::setNumberOfRows (int rows)
{
	data.numRows = rows;
}

/*-----------------------------------------------------------------------**/
void image::setNumberOfColumns (int columns)
{
	data.numColumns = columns;
}

/*-------------------------------------------------------------------*/
void image::setPixel(const int row, const int col, const int value) 
{
	data.redChannel [row*data.numColumns+col] = value; 
}


/*-------------------------------------------------------------------*/
void image::setPixel(const int row, const int col, const int rgb, const int value)
{
	switch (rgb) 
	{
		case RED: data.redChannel [row*data.numColumns+col] = value; break;
		case GREEN: data.greenChannel [row*data.numColumns+col] = value; break;
		case BLUE: data.blueChannel [row*data.numColumns+col] = value; break;
	}
}

/*-------------------------------------------------------------------*/
int image::getPixel(const int row, const int col) 
{ 
	return data.redChannel [row*data.numColumns+col]; 
}

/*-------------------------------------------------------------------*/
int image::getPixel(const int row, const int col, const int rgb) 
{
	int value;
	switch (rgb)
	{
		case RED: value = data.redChannel [row*data.numColumns+col]; break;
		case GREEN: value = data.greenChannel [row*data.numColumns+col]; break;
		case BLUE: value = data.blueChannel [row*data.numColumns+col]; break;
	}
	return value;
}

/*-----------------------------------------------------------------------**/
int image::getNumberOfRows () 
{
	return data.numRows;
}

/*-----------------------------------------------------------------------**/
int image::getNumberOfColumns () 
{
	return data.numColumns;
}


/*-----------------------------------------------------------------------**/
vector<int>* image::getChannel (int rgb) 
{
	switch (rgb) {
		case RED:
		return &data.redChannel;
		break;
		case GREEN:
		return &data.greenChannel;
		break;
		case BLUE:
		return &data.blueChannel;
		break;
	}
}
	/*-----------------------------------------------------------------------**/
bool image::setChannel (int rgb, vector<int> &channel)
{
	if (channel.size() == this->getChannel(rgb)->size())
	{
		*(this->getChannel(rgb)) = channel;
		return true;
	}
	return false;
}


/*-------------------------------------------------------------------*/
bool image::save (const char* file)
{
	fstream outfile;
	int flag;
	
	outfile.open(file, std::ios_base::out | std::ios_base::binary);
	if (!outfile.is_open())
	{
		fprintf(stderr, "\nCannot open image file %s\n", file);
		return false;
	}

	if (strstr(file, ".ppm") != NULL) 
	{	/* PPM Color Image */
		flag = 0;
	}

	else if (strstr(file, ".pgm") != NULL)
	{	/* PGM Gray-level Image */ 
		flag = 1;
	}
	else
	{
		fprintf(stderr, "\nIncorrect File Type\n");
		return false;
	}

	if (flag)
	{
		outfile << "P5\n";
		outfile << data.numColumns << " " << data.numRows << "\n";
		outfile << "255\n";
	}
	else
	{
		outfile << "P6\n";
		outfile << data.numColumns << " " << data.numRows << "\n";
		outfile << "255\n";
	}

	if (flag) 
	{
		for (int i=0; i < data.numRows; i++) 
		{
			for (int j=0; j < data.numColumns; j++)
			{
				outfile << (unsigned char)(((data.redChannel[i * data.numColumns + j]) > 255) ? 255 : (data.redChannel[i * data.numColumns + j]));
			}
		}
	}
	else {
		for (int i=0; i < data.numRows; i++) 
		{
			for (int j=0; j < data.numColumns; j++) 
			{
				outfile << (unsigned char)(((data.redChannel[i*data.numColumns + j])> 255) ? 255 : (data.redChannel[i*data.numColumns + j]));
				outfile << (unsigned char)(((data.greenChannel[i*data.numColumns + j])> 255) ? 255 : (data.greenChannel[i*data.numColumns + j]));
				outfile << (unsigned char)(((data.blueChannel[i*data.numColumns + j]) > 255) ? 255 : (data.blueChannel[i*data.numColumns + j]));
			}
		}
	}

	outfile.close();
	return true;
}

/*-------------------------------------------------------------------*/
bool image::save (char* file)
{
	const char* name = file;
	save(name);

	return true;
}
	/*-------------------------------------------------------------------*/

bool image::read (char * filename) 
{
	fstream pgm_file;
	string str;
	unsigned char r, g, b;
	int i,j, flag;
	
	if (filename == (char*)NULL) {
		fprintf(stderr, "\nNeed a imput\n");
		exit(1);
	}

	if (strstr(filename, ".ppm") != NULL) 
	{	/* PPM Color Image */
		flag = 0;
	}

	else if (strstr(filename, ".pgm") != NULL)
	{	/* PGM Gray-level Image */ 
		flag = 1;
	}
	else
	{
		fprintf(stderr, "\nIncorrect File Type\n");
		return false;
	}

	pgm_file.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!pgm_file.is_open()) {
		fprintf(stderr, "\nCannot open image file %s\n", filename);
		exit(1);
	}

	std::getline(pgm_file, str);

	if ((flag == 0) && (strcmp (str.c_str(), "P6") != 0)) {
		fprintf(stderr, "\n image file %s not in PPM format...%s", filename, str.c_str());
		return false;
	}
	if ((flag == 1) && (strcmp (str.c_str(), "P5") != 0)) {
		fprintf(stderr, "\n image file %s not in PGM format...%s", filename, str.c_str());
		return false;
	}
	
	int col, row;
	
	/* check for commant line */
	char temp = pgm_file.peek();
	if (isdigit(temp)) {
		pgm_file >> col;
		pgm_file >> row;
	}
	else {
		getline(pgm_file, str);		// throw the comment away. 
		pgm_file >> col;
		pgm_file >> row;
	}

	data.numRows = row;
	data.numColumns = col;
	resize(data.numRows, data.numColumns);

	string line;
	getline(pgm_file,line);
	getline(pgm_file,line);
	
	if (flag)
	{	/* Gray-level Image */
		char * buffer = new char [row*col];
		pgm_file.read(buffer, row*col);

		for(i = 0; i < row*col; i++){

			r = (unsigned char)buffer[i];
			data.redChannel [i] = r;
			data.greenChannel [i] = r;
			data.blueChannel [i] = r;

		}

		delete[] buffer;

	}
	else 
	{	/* Color Image */
		char * buffer = new char [row*col*3];
		pgm_file.read(buffer, row*col*3);

		for(i = 0; i < row*col; i++){

			r = (unsigned char)buffer[i*3];
			g = (unsigned char)buffer[i*3+1];
			b = (unsigned char)buffer[i*3+2];
			data.redChannel [i] = r;
			data.greenChannel [i] = g;
			data.blueChannel [i] = b;
			
		}
		
		delete[] buffer;
	} 

	pgm_file.close();
	return true;
}

/*-----------------------------------------------------------------------**/
int image::getint(FILE *fp) 
{
	int item, i, flag;

/* skip forward to start of next number */
	item  = getc(fp); flag = 1;
	do {

		if (item =='#') {   /* comment*/
			while (item != '\n' && item != EOF) item=getc(fp);
		}

		if (item ==EOF) return 0;
		if (item >='0' && item <='9') 
			{flag = 0;  break;}

	/* illegal values */
		if ( item !='\t' && item !='\r' 
			&& item !='\n' && item !=',') return(-1);

		item = getc(fp);
	} while (flag == 1);


/* get the number*/
	i = 0; flag = 1;
	do {
		i = (i*10) + (item - '0');
		item = getc(fp);
		if (item <'0' || item >'9') {
			flag = 0; break;}
		if (item==EOF) break; 
	} while (flag == 1);

	return i;
}

vector<double> image::RGBtoHSI(double r, double g, double b){

	double i = (r + g + b) / 3.0;

	double rn = r / (r + g + b);
	double gn = g / (r + g + b);
	double bn = b / (r + g + b);

	double h = acos((0.5 * ((rn - gn) + (rn - bn))) / (sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
	if(b > g)
	{
		h = 2 * M_PI - h;	
	}

	double s = 1 - 3 * min(rn, min(gn, bn));

	return {h,s,i};
}

vector<double> image::HSItoRGB(double h, double s, double i){

	double x = i * (1 - s);
	double r, g, b;
	// For black, white and grayscale h is NaN. Conversion works incorrectly.
	if(std::isnan(h))
	{
	  r = i;
	  g = i;
	  b = i;
	}
	else if(h < 2 * M_PI / 3)
	{
		double y = i * (1 + (s * cos(h)) / (cos(M_PI / 3 - h)));
		double z = 3 * i - (x + y);
		b = x; r = y; g = z;
	}
	else if(h < 4 * M_PI / 3)
	{
		double y = i * (1 + (s * cos(h - 2 * M_PI / 3)) / (cos(M_PI / 3 - (h  - 2 * M_PI / 3))));
		double z = 3 * i - (x + y);
		r = x; g = y; b = z;
	}
	else
	{
		double y = i * (1 + (s * cos(h - 4 * M_PI / 3)) / (cos(M_PI / 3 - (h  - 4 * M_PI / 3))));
		double z = 3 * i - (x + y);
		r = z; g = x; b = y;
	}

	
	return {r,g,b};
}

void image::convertToHSI(){
	if(HSI == true){
		cout << "Already in HSI" << endl;
		return;
	}

	for(int i = 0; i < data.numRows; i++){
		for(int j = 0; j < data.numColumns; j++){
			//get rgb values
			int r = getPixel(i,j,RED);
			int g = getPixel(i,j,GREEN);
			int b = getPixel(i,j,BLUE);

			//convert to hsi
			vector<double> hsi = RGBtoHSI(r,g,b);

			// denormalize hsi values
			int H = round(hsi[HUE] * 180.0 / M_PI);
			int S = round(hsi[SATURATION]*100);
			int I = round(hsi[INTENSITY]);
			
			//set channels
			setPixel(i,j,HUE,H);
			setPixel(i,j,SATURATION,S);
			setPixel(i,j,INTENSITY,I);
		}
	}
	HSI = true;
}

void image::convertToRGB(){
	if(HSI == false){
		cout << "Already in RGB" << endl;
		return;
	}

	for(int i = 0; i < data.numRows; i++){
		for(int j = 0; j < data.numColumns; j++){
			//get normalized HSI values
			double H = getPixel(i,j,HUE)*M_PI/180.0;
			double S = getPixel(i,j,SATURATION)/100.0;
			double I = getPixel(i,j,INTENSITY);

			//convert to RGB
			vector<double> rgb = HSItoRGB(H,S,I);

			//set channels
			setPixel(i,j,RED,round(rgb[RED]));
			setPixel(i,j,GREEN,round(rgb[GREEN]));
			setPixel(i,j,BLUE,round(rgb[BLUE]));
		}
	}
	HSI = false;
}