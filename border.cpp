//Date:February 3,2017
//Name:Tian Ye
//ID:2581-2031-06
//Email:tye459@usc.edu

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "border.h"


//输入数组长度大于1000000，图片长宽均不大于1000，可修改临时数组矩阵大小以满足更大的图片
//扩边一层
void Border_extend(int original_array[], int out_array[], int width, int height)  
{   
	int B[1000][1000];
	int C[1000][1000];
	int A[1000 * 1000];
	int Aout[1000*1000] = { 0 };
	int Aout2[1000*1000] = { 0 };

    //输入是A[i]
	for (int i = 0; i < width * height; i++) 
	{
		A[i] = original_array[i];
	}
	//全部后移一行
	for (int i = 0; i < width * height; i++)
	{
		Aout[i + width] = A[i];
	}  
	//补齐最后一行
	for (int i = 0; i < width; i++)
	{
		Aout[i] = Aout[i + width];
	}
	//补齐第一行
	for (int i = 0; i < width; i++)
	{
		Aout[width * height + width + i] = Aout[width*height + i];
	}

	//转化为矩阵
	for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			B[i][j] = Aout[width * i + j];
		}
	}
	//矩阵转置
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			C[i][j] = B[j][i];
		}
	}
	//转化回数组
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			Aout[(height + 2)* i + j] = C[i][j];
		}
	}

	//把数组全部后移一行
	for (int i = 0; i < width * (height + 2); i++)
	{
		Aout2[i + (height + 2)] = Aout[i];
	}
    //补齐最后一行
	for (int i = 0; i < (height + 2); i++)
	{
		Aout2[(width + 1) * (height + 2) + i] = Aout2[width * (height + 2) + i];
	}
	//补齐第一行
	for (int i = 0; i < height + 2; i++)
	{
		Aout2[i] = Aout2[height + 2 + i];
	}

	//转化为矩阵
	for (int i = 0; i < width + 2; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			B[i][j] = Aout2[(height + 2) * i + j];
		}
	}
	//矩阵转置
    for (int i = 0; i < width + 2; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			C[j][i] = B[i][j];
		}
	}
	//转化回数组
	for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			Aout[(width + 2) * i + j] = C[i][j];
		}
	}
	//输出是Aout
	for (int i = 0; i < (width + 2)*(height + 2);i++)
	{
		out_array[i] = Aout[i];
	} 
}
