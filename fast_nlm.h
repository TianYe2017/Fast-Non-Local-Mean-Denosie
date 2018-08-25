#pragma once
#ifndef FAST_NLM_H
#define FAST_NLM_H
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "psnr.h"
#include "border.h"

void Create_gaussian_core(double output[], double SD, int size);
double Gaussian_distance_weight(int array1[], int array2[], double weight[], double SD, int size);
void Fast_nlm_7_23(int input[], int output[], int width, int height, double SD,double gaussian_k);
void Fast_nlm_9_47(int input[], int output[], int width, int height, double SD,double gaussian_k);


#endif
