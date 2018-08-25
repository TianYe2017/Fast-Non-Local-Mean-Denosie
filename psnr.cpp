//Date:February 3,2017
//Name:Tian Ye
//ID:2581-2031-06
//Email:tye459@usc.edu

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "psnr.h"
#include "math.h"


double Find_PSNR(int original_array[], int output_array[], int width, int height) 
{   
	double temp;
	double MSE = 0.0;
	double PSRN;
	for (int i = 0; i < width * height; i++)
	{
		temp = (double)(original_array[i] - output_array[i]);
		temp = pow(temp, 2);
		MSE = MSE + temp;
	}
	MSE = MSE / (double)(width * height);
	PSRN = 10.0 * log10((255.0 * 255.0) / MSE);
	return PSRN;
}