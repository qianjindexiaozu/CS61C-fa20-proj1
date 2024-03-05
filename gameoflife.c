/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**				Andy Wang
**
** DATE:        2020-08-23
**				2024-03-05
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	int place = row * image->cols + col;
	Color *new_color = (Color *)malloc(sizeof(Color));
	int countR = 0, countG = 0, countB = 0, nowR = 0, nowG = 0, nowB = 0;

	if((*(image->image + place))->R)	nowR = 1;
	if((*(image->image + place))->G)	nowG = 1;
	if((*(image->image + place))->B)	nowB = 1;

	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(i == 0 && j == 0) continue;
			int temp = (i + row + image->rows) % image->rows * image->cols + (col + j + image->cols) % image->cols;
			if((*(image->image + temp))->R) countR++;
			if((*(image->image + temp))->G) countG++;
			if((*(image->image + temp))->B) countB++;
		}
	}


	uint32_t state_R = 1, state_G = 1, state_B = 1;
	state_R = state_R << (nowR * 9 + countR);
	state_G = state_G << (nowG * 9 + countG);
	state_B = state_B << (nowB * 9 + countB);
	if(state_R & rule){
		new_color->R = 255;
	}
	else {
		new_color->R = 0;
	}
	if(state_G & rule){
		new_color->G = 255;
	}
	else {
		new_color->G = 0;
	}
	if(state_B & rule){
		new_color->B = 255;
	}
	else {
		new_color->B = 0;
	}
	return new_color;

	
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* new_image = (Image *)malloc(sizeof(Image));
	new_image->image = (Color **)malloc(sizeof(Color*) * image->cols * image->rows);
	new_image->cols = image->cols;
	new_image->rows = image->rows;
	Color **p = new_image->image;
	for(int i = 0; i < image->rows; i++){
		for(int j = 0; j < image->cols; j++){
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc != 3 || argv[2][0] != '0' || argv[2][1] != 'x'){
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	uint32_t rule = strtol(argv[2], NULL, 16);
	Image* startedImage = readData(argv[1]);
	if(!startedImage){
		printf("read date error!\n");
		exit(-1);
	}
	Image* lifeImage = life(startedImage, rule);
	if(!lifeImage){
		printf("life error!\n");
		exit(-1);
	}
	writeData(lifeImage);
	freeImage(startedImage);
	freeImage(lifeImage);
	return 0;

}
