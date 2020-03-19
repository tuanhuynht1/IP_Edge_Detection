IP Project 3: Edge Detection
Author: Tuan Huynh
Contributor: Chih-Yun Pai

--------------------------------- LINUX BUILD AND ARCHITECTURE ---------------------------------

Project/                    //main project directory
    image/                  //directory containing source code for image data structure
        image.h
        image.cpp
    utility/                //directory containing source code for edge detection functions
        utility.h
        utility.cpp
    Driver.cpp              //program containing the main function
    Makefile                //makefile to for creating iptools application
    parameters.txt          //sample input file that uses all the image files listed below
    lena.pgm 
    tree.pgm
    wheel.pgm
    frog.ppm
    004.ppm
    garden.ppm

To create the application on a LINUX machine, run the following command:
g++ -std=c++11 -o iptools Driver.cpp image/image.cpp utility/utility.cpp
or use the Makefile by running the 'make' command in the Project directory 

This will result in an iptools executable

To execute the program, run the following command:
./iptools [parameters file]

The parameters file must be a text file with the correct format.
The file and all the images it references must be in the same directory as iptools.

----------------------------------- PARAMETERS FILE FORMAT -----------------------------------

Every operation will have one operation line followed by 1-3 region specification line(s).
Multiple operations can be preformed by appending operations with no blank lines in between.
Everyline will have specific arguments separated by a single space.

Format for the operation line is as follows:
[source image] [name] [opcode] [# of regions]

source image: filename for source image, i.e. tree.pgm
name: name that serves as the basis for the output file name (do not include a '.' or an extension)
opcode: specifies which operation to preform, valid opcodes are as follows
    GSCA: Grey-scale Amplitude Edge 
    GSCT: Grey-scale Thresholded Edge 
    GSCD: Grey-scale Edge Direction 
    RGBT: RGB Thresholded Edge Combination 
    HSIA: HSI Amplitude Edge
    HSIT: HSI Thresholded Edge
# of regions: number of regions to operate on (1-3) must match # of following region specification lines 

Region specification lines will have different formats based on the opcode.
Format for the region specification lines according to opcode is as follows:

GSCA:
[i0] [j0] [ilen] [jlen] [mask type]
GSCT:
[i0] [j0] [ilen] [jlen] [mask type] [threshold]
GSCD:
[i0] [j0] [ilen] [jlen] [mask type] [threshold] [degree]
RGBT:
[i0] [j0] [ilen] [jlen] [mask type] [threshold]
HSIA:
[i0] [j0] [ilen] [jlen] [mask type] [channel]
HSIT:
[i0] [j0] [ilen] [jlen] [mask type] [threshold] [channel]

i0: starting i index of region (top-left corner)
j0: starting j index of region (top-left corner)
ilen: region's height
jlen: region's width
mask type: size of Sobel kernel, [S3] for 3x3 sobel operator, [S5] for 5x5 sobel operator
threshold: threshold value [0,255]
degree: direction angle [0,180]
channel: specific HSI channel to operate on, [H] for hue, [S] for saturation, [I] for intensity

example of a multiple operations parameter file...

frog.ppm frog HSIT 2        //perform HSI thresholded edge on 2 regions 
0 0 140 300 S3 25 I         //uses a 3x3 sobel operator on intensity channel with threshold value of 25
160 0 140 300 S5 45 S       //uses a 5x5 sobel operator on saturation channel with threshold value of 45
wheel.pgm wheel GSCD 1      //perform Grey-scale edge direction on 1 region 
30 30 200 200 S3 55 135     //uses a 3x3 sobel operator with a threshold value of 55, detects 135 degree edges

-------------------------------------------------- OUTPUT FILES --------------------------------------------------

Output images produced accoring to operation are as follows:

GSCA: Grey-scale Amplitude Edge
    [name]_amplitude.pgm

GSCT: Grey-scale Thresholded Edge
    [name]_threshold.pgm

GSCD: Grey-scale Edge Direction
    [name]_direction.pgm

RGBT: RGB Thresholded Edge Combination
    [name]_threshold_R.pgm
    [name]_threshold_G.pgm
    [name]_threshold_B.pgm
    [name]_threshold_RGB.ppm

HSIA: HSI Amplitude Edge
    [name]_amplitude_HSI.ppm

HSIT: HSI Thresholded Edge
    [name]_threshold_HSI.ppm

