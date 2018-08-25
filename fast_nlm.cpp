//Date:February 3,2017
//Name:Tian Ye
//ID:2581-2031-06
//Email:tye459@usc.edu


#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "fast_nlm.h"


void Create_gaussian_core(double output[], double SD, int size) 
{   
	double temp = 0.0;
	double temp1 = 0.0;
	int band = (size - 1) / 2;
	for (int i = -band; i <= band; i++) 
	{
		for (int j = -band; j <= band; j++) 
		{
			temp = -(double)(i * i + j * j) / (double)(2 * SD *SD);
			output[size * (i + band) + j + band] = exp(temp);
			temp1 = temp1 + exp(temp);
		}
	}
	for (int i = 0; i <= size * size; i++)
	{
		output[i] = output[i] / temp1;
	}	
}

double Gaussian_distance_weight(int array1[], int array2[], double weight[], double SD, int size)
{   
	double temp = 0.0;
	for (int i = 0; i < size * size; i++) 
	{
		temp = temp + (double)((array1[i] - array2[i]) * (array1[i] - array2[i])) * weight[i];
	}
	temp = exp(-temp / (2 * SD * SD));
	return temp;
}

void Fast_nlm_7_23(int input[], int output[], int width, int height, double SD, double gaussian_k) 
{  
	int temp[1000 * 1000];  //514*514
	int temp2[1000 * 1000];
	int temp_m_in[1000][1000]; //514*514
	int temp_m_out[1000][1000];
	int temp_a_out[1000 * 1000];
	double weight_table[100*100];
	double adjusted_weight_table[100 * 100];
	int window_data1[7 * 7];
	int window_data2[7 * 7];
	double total_weight = 0.0;
	double filtered_value;
	double count = 0;
	double gaussian_core[49] = {0};
	int index;

	Create_gaussian_core(gaussian_core, gaussian_k, 7);


	//先扩边 MDZZ居然要扩11次
	Border_extend(input, temp, width, height);          //1
	Border_extend(temp, temp2, width + 2, height + 2);  //2
	Border_extend(temp2, temp, width + 4, height + 4);  //3
	Border_extend(temp, temp2, width + 6, height + 6);  //4
	Border_extend(temp2, temp, width + 8, height + 8);  //5
	Border_extend(temp, temp2, width + 10, height + 10);  //6 
	Border_extend(temp2, temp, width + 12, height + 12);  //7
	Border_extend(temp, temp2, width + 14, height + 14);  //8 
	Border_extend(temp2, temp, width + 16, height + 16);  //9
	Border_extend(temp, temp2, width + 18, height + 18);  //10
	Border_extend(temp2, temp, width + 20, height + 20);  //11

	//转化为矩阵
	for (int i = 0; i < height + 22; i++)
	{
		for (int j = 0; j < width + 22; j++)
		{
			temp_m_in[i][j] = temp[(width + 22) * i + j];
		}
	}
    
	//
	
	for (int i = 11; i < height + 11; i++) 
	{   
		count++;
		printf("\n Already finish %lf percent", count / 512.0*100.0);

		
		for (int j = 11; j < width + 11; j++) 
		{   
			
			window_data1[0] = temp_m_in[i - 3][j - 3];
			window_data1[1] = temp_m_in[i - 3][j - 2];
			window_data1[2] = temp_m_in[i - 3][j - 1];
			window_data1[3] = temp_m_in[i - 3][j];
			window_data1[4] = temp_m_in[i - 3][j + 1];
			window_data1[5] = temp_m_in[i - 3][j + 2];
			window_data1[6] = temp_m_in[i - 3][j + 3];

			window_data1[7] = temp_m_in[i - 2][j - 3];
			window_data1[8] = temp_m_in[i - 2][j - 2];
			window_data1[9] = temp_m_in[i - 2][j - 1];
			window_data1[10] = temp_m_in[i - 2][j];
			window_data1[11] = temp_m_in[i - 2][j + 1];
			window_data1[12] = temp_m_in[i - 2][j + 2];
			window_data1[13] = temp_m_in[i - 2][j + 3];

			window_data1[14] = temp_m_in[i - 1][j - 3];
			window_data1[15] = temp_m_in[i - 1][j - 2];
			window_data1[16] = temp_m_in[i - 1][j - 1];
			window_data1[17] = temp_m_in[i - 1][j];
			window_data1[18] = temp_m_in[i - 1][j + 1];
			window_data1[19] = temp_m_in[i - 1][j + 2];
			window_data1[20] = temp_m_in[i - 1][j + 3];

			window_data1[21] = temp_m_in[i][j - 3];
			window_data1[22] = temp_m_in[i][j - 2];
			window_data1[23] = temp_m_in[i][j - 1];
			window_data1[24] = temp_m_in[i][j];
			window_data1[25] = temp_m_in[i][j + 1];
			window_data1[26] = temp_m_in[i][j + 2];
			window_data1[27] = temp_m_in[i][j + 3];

			window_data1[28] = temp_m_in[i + 1][j - 3];
			window_data1[29] = temp_m_in[i + 1][j - 2];
			window_data1[30] = temp_m_in[i + 1][j - 1];
			window_data1[31] = temp_m_in[i + 1][j];
			window_data1[32] = temp_m_in[i + 1][j + 1];
			window_data1[33] = temp_m_in[i + 1][j + 2];
			window_data1[34] = temp_m_in[i + 1][j + 3];

			window_data1[35] = temp_m_in[i + 2][j - 3];
			window_data1[36] = temp_m_in[i + 2][j - 2];
			window_data1[37] = temp_m_in[i + 2][j - 1];
			window_data1[38] = temp_m_in[i + 2][j];
			window_data1[39] = temp_m_in[i + 2][j + 1];
			window_data1[40] = temp_m_in[i + 2][j + 2];
			window_data1[41] = temp_m_in[i + 2][j + 3];

			window_data1[42] = temp_m_in[i + 3][j - 3];
			window_data1[43] = temp_m_in[i + 3][j - 2];
			window_data1[44] = temp_m_in[i + 3][j - 1];
			window_data1[45] = temp_m_in[i + 3][j];
			window_data1[46] = temp_m_in[i + 3][j + 1];
			window_data1[47] = temp_m_in[i + 3][j + 2];
			window_data1[48] = temp_m_in[i + 3][j + 3];

			//printf("ind");
		    //外循环遍历矩阵内的所有数据
			//内循环遍历搜索窗内的数据
			index = 0;
			for (int y = i - 11 + 3; y <= i + 11 - 3; y++) 
			{
			//	printf("done");
				for (int x = j - 11 + 3; x <= j + 11 - 3; x++) 
				{
					window_data2[0] = temp_m_in[y - 3][x - 3];
					window_data2[1] = temp_m_in[y - 3][x - 2];
					window_data2[2] = temp_m_in[y - 3][x - 1];
					window_data2[3] = temp_m_in[y - 3][x];
					window_data2[4] = temp_m_in[y - 3][x + 1];
					window_data2[5] = temp_m_in[y - 3][x + 2];
					window_data2[6] = temp_m_in[y - 3][x + 3];

					window_data2[7] = temp_m_in[y - 2][x - 3];
					window_data2[8] = temp_m_in[y - 2][x - 2];
					window_data2[9] = temp_m_in[y - 2][x - 1];
					window_data2[10] = temp_m_in[y - 2][x];
					window_data2[11] = temp_m_in[y - 2][x + 1];
					window_data2[12] = temp_m_in[y - 2][x + 2];
					window_data2[13] = temp_m_in[y - 2][x + 3];

					window_data2[14] = temp_m_in[y - 1][x - 3];
					window_data2[15] = temp_m_in[y - 1][x - 2];
					window_data2[16] = temp_m_in[y - 1][x - 1];
					window_data2[17] = temp_m_in[y - 1][x];
					window_data2[18] = temp_m_in[y - 1][x + 1];
					window_data2[19] = temp_m_in[y - 1][x + 2];
					window_data2[20] = temp_m_in[y - 1][x + 3];

					window_data2[21] = temp_m_in[y][x - 3];
					window_data2[22] = temp_m_in[y][x - 2];
					window_data2[23] = temp_m_in[y][x - 1];
					window_data2[24] = temp_m_in[y][x];
					window_data2[25] = temp_m_in[y][x + 1];
					window_data2[26] = temp_m_in[y][x + 2];
					window_data2[27] = temp_m_in[y][x + 3];

					window_data2[28] = temp_m_in[y + 1][x - 3];
					window_data2[29] = temp_m_in[y + 1][x - 2];
					window_data2[30] = temp_m_in[y + 1][x - 1];
					window_data2[31] = temp_m_in[y + 1][x];
					window_data2[32] = temp_m_in[y + 1][x + 1];
					window_data2[33] = temp_m_in[y + 1][x + 2];
					window_data2[34] = temp_m_in[y + 1][x + 3];

					window_data2[35] = temp_m_in[y + 2][x - 3];
					window_data2[36] = temp_m_in[y + 2][x - 2];
					window_data2[37] = temp_m_in[y + 2][x - 1];
					window_data2[38] = temp_m_in[y + 2][x];
					window_data2[39] = temp_m_in[y + 2][x + 1];
					window_data2[40] = temp_m_in[y + 2][x + 2];
					window_data2[41] = temp_m_in[y + 2][x + 3];

					window_data2[42] = temp_m_in[y + 3][x - 3];
					window_data2[43] = temp_m_in[y + 3][x - 2];
					window_data2[44] = temp_m_in[y + 3][x - 1];
					window_data2[45] = temp_m_in[y + 3][x];
					window_data2[46] = temp_m_in[y + 3][x + 1];
					window_data2[47] = temp_m_in[y + 3][x + 2];
					window_data2[48] = temp_m_in[y + 3][x + 3];

					index++;
					weight_table[index] = Gaussian_distance_weight(window_data1, window_data2, gaussian_core, SD, 7);		
					//printf("\n dhfhf %d", index);
				}				
			}	

		
			total_weight = 0.0;
			for (int r = 0; r < 17 * 17; r++)
			{
				total_weight += weight_table[r];
			}
			//double test = 0.0;
			for (int k = 0; k < 17 * 17; k++)
			{
				adjusted_weight_table[k] = weight_table[k] / total_weight;
				//test += adjusted_weight_table[k];
			}
			filtered_value = 0.0;
			index = 0;
			for (int y1 = i - 11 + 3; y1 <= i + 11 - 3; y1++)
			{
				for (int x1 = j - 11 + 3; x1 <= j + 11 - 3; x1++)
				{
					filtered_value = filtered_value + (double)temp_m_in[y1][x1] * adjusted_weight_table[index];
					index++;				
					//printf("\n test is %lf", filtered_value);
				}
			}
			
			output[width * (i - 11) + j - 11] = (int)filtered_value;
	
		}
	}

}

void Fast_nlm_9_47(int input[], int output[], int width, int height, double SD , double gaussian_k)
{
	int temp[1000 * 1000];  //514*514
	int temp2[1000 * 1000];
	int temp_m_in[1000][1000]; //514*514
	int temp_m_out[1000][1000];
	int temp_a_out[1000 * 1000];
	double weight_table[100 * 100];
	double adjusted_weight_table[100 * 100];
	int window_data1[9 * 9];
	int window_data2[9 * 9];
	double total_weight = 0.0;
	double filtered_value;
	double count = 0;
	double gaussian_core[81] = { 0 };
	int index;

	Create_gaussian_core(gaussian_core, gaussian_k, 9);


	//先扩边 MDZZ居然要扩23次
	Border_extend(input, temp, width, height);          //1
	Border_extend(temp, temp2, width + 2, height + 2);  //2
	Border_extend(temp2, temp, width + 4, height + 4);  //3
	Border_extend(temp, temp2, width + 6, height + 6);  //4
	Border_extend(temp2, temp, width + 8, height + 8);  //5
	Border_extend(temp, temp2, width + 10, height + 10);  //6 
	Border_extend(temp2, temp, width + 12, height + 12);  //7
	Border_extend(temp, temp2, width + 14, height + 14);  //8 
	Border_extend(temp2, temp, width + 16, height + 16);  //9
	Border_extend(temp, temp2, width + 18, height + 18);  //10
	Border_extend(temp2, temp, width + 20, height + 20);  //11
	Border_extend(temp, temp2, width + 22, height + 22);  //12
	Border_extend(temp2, temp, width + 24, height + 24);  //13
	Border_extend(temp, temp2, width + 26, height + 26);  //14
	Border_extend(temp2, temp, width + 28, height + 28);  //15
	Border_extend(temp, temp2, width + 30, height + 30);  //16
	Border_extend(temp2, temp, width + 32, height + 32);  //17
	Border_extend(temp, temp2, width + 34, height + 34);  //18
	Border_extend(temp2, temp, width + 36, height + 36);  //19
	Border_extend(temp, temp2, width + 38, height + 38);  //20
	Border_extend(temp2, temp, width + 40, height + 40);  //21
	Border_extend(temp, temp2, width + 42, height + 42);  //22
	Border_extend(temp2, temp, width + 44, height + 44);  //23

	 //转化为矩阵
	for (int i = 0; i < height + 46; i++)
	{
		for (int j = 0; j < width + 46; j++)
		{
			temp_m_in[i][j] = temp[(width + 46) * i + j];
		}
	}

	//

	for (int i = 23; i < height + 23; i++)
	{
		count++;
		printf("\n Already finish %lf percent", count / 512.0*100.0);


		for (int j = 23; j < width + 23; j++)
		{

			window_data1[0] = temp_m_in[i - 4][j - 4];
			window_data1[1] = temp_m_in[i - 4][j - 3];
			window_data1[2] = temp_m_in[i - 4][j - 2];
			window_data1[3] = temp_m_in[i - 4][j - 1];
			window_data1[4] = temp_m_in[i - 4][j];
			window_data1[5] = temp_m_in[i - 4][j + 1];
			window_data1[6] = temp_m_in[i - 4][j + 2];
			window_data1[7] = temp_m_in[i - 4][j + 3];
			window_data1[8] = temp_m_in[i - 4][j + 4];

			window_data1[9] = temp_m_in[i - 3][j - 4];
			window_data1[10] = temp_m_in[i - 3][j - 3];
			window_data1[11] = temp_m_in[i - 3][j - 2];
			window_data1[12] = temp_m_in[i - 3][j - 1];
			window_data1[13] = temp_m_in[i - 3][j];
			window_data1[14] = temp_m_in[i - 3][j + 1];
			window_data1[15] = temp_m_in[i - 3][j + 2];
			window_data1[16] = temp_m_in[i - 3][j + 3];
			window_data1[17] = temp_m_in[i - 3][j + 4];

			window_data1[18] = temp_m_in[i - 2][j - 4];
			window_data1[19] = temp_m_in[i - 2][j - 3];
			window_data1[20] = temp_m_in[i - 2][j - 2];
			window_data1[21] = temp_m_in[i - 2][j - 1];
			window_data1[22] = temp_m_in[i - 2][j];
			window_data1[23] = temp_m_in[i - 2][j + 1];
			window_data1[24] = temp_m_in[i - 2][j + 2];
			window_data1[25] = temp_m_in[i - 2][j + 3];
			window_data1[26] = temp_m_in[i - 2][j + 4];

			window_data1[27] = temp_m_in[i - 1][j - 4];
			window_data1[28] = temp_m_in[i - 1][j - 3];
			window_data1[29] = temp_m_in[i - 1][j - 2];
			window_data1[30] = temp_m_in[i - 1][j - 1];
			window_data1[31] = temp_m_in[i - 1][j];
			window_data1[32] = temp_m_in[i - 1][j + 1];
			window_data1[33] = temp_m_in[i - 1][j + 2];
			window_data1[34] = temp_m_in[i - 1][j + 3];
			window_data1[35] = temp_m_in[i - 1][j + 4];

			window_data1[36] = temp_m_in[i][j - 4];
			window_data1[37] = temp_m_in[i][j - 3];
			window_data1[38] = temp_m_in[i][j - 2];
			window_data1[39] = temp_m_in[i][j - 1];
			window_data1[40] = temp_m_in[i][j];
			window_data1[41] = temp_m_in[i][j + 1];
			window_data1[42] = temp_m_in[i][j + 2];
			window_data1[43] = temp_m_in[i][j + 3];
			window_data1[44] = temp_m_in[i][j + 4];

			window_data1[45] = temp_m_in[i + 1][j - 4];
			window_data1[46] = temp_m_in[i + 1][j - 3];
			window_data1[47] = temp_m_in[i + 1][j - 2];
			window_data1[48] = temp_m_in[i + 1][j - 1];
			window_data1[49] = temp_m_in[i + 1][j];
			window_data1[50] = temp_m_in[i + 1][j + 1];
			window_data1[51] = temp_m_in[i + 1][j + 2];
			window_data1[52] = temp_m_in[i + 1][j + 3];
			window_data1[53] = temp_m_in[i + 1][j + 4];
		
			window_data1[54] = temp_m_in[i + 2][j - 4];
			window_data1[55] = temp_m_in[i + 2][j - 3];
			window_data1[56] = temp_m_in[i + 2][j - 2];
			window_data1[57] = temp_m_in[i + 2][j - 1];
			window_data1[58] = temp_m_in[i + 2][j];
			window_data1[59] = temp_m_in[i + 2][j + 1];
			window_data1[60] = temp_m_in[i + 2][j + 2];
			window_data1[61] = temp_m_in[i + 2][j + 3];
			window_data1[62] = temp_m_in[i + 2][j + 4];

			window_data1[63] = temp_m_in[i + 3][j - 4];
			window_data1[64] = temp_m_in[i + 3][j - 3];
			window_data1[65] = temp_m_in[i + 3][j - 2];
			window_data1[66] = temp_m_in[i + 3][j - 1];
			window_data1[67] = temp_m_in[i + 3][j];
			window_data1[68] = temp_m_in[i + 3][j + 1];
			window_data1[69] = temp_m_in[i + 3][j + 2];
			window_data1[70] = temp_m_in[i + 3][j + 3];
			window_data1[71] = temp_m_in[i + 3][j + 4];

			window_data1[72] = temp_m_in[i + 4][j - 4];
			window_data1[73] = temp_m_in[i + 4][j - 3];
			window_data1[74] = temp_m_in[i + 4][j - 2];
			window_data1[75] = temp_m_in[i + 4][j - 1];
			window_data1[76] = temp_m_in[i + 4][j];
			window_data1[77] = temp_m_in[i + 4][j + 1];
			window_data1[78] = temp_m_in[i + 4][j + 2];
			window_data1[79] = temp_m_in[i + 4][j + 3];
			window_data1[80] = temp_m_in[i + 4][j + 4];


			//printf("ind");
			//外循环遍历矩阵内的所有数据
			//内循环遍历搜索窗内的数据
			index = 0;
			for (int y = i - 23 + 4; y <= i + 23 - 4; y++)
			{
				//	printf("done");
				for (int x = j - 23 + 4; x <= j + 23 - 4; x++)
				{
					window_data2[0] = temp_m_in[y - 4][x - 4];
					window_data2[1] = temp_m_in[y - 4][x - 3];
					window_data2[2] = temp_m_in[y - 4][x - 2];
					window_data2[3] = temp_m_in[y - 4][x - 1];
					window_data2[4] = temp_m_in[y - 4][x];
					window_data2[5] = temp_m_in[y - 4][x + 1];
					window_data2[6] = temp_m_in[y - 4][x + 2];
					window_data2[7] = temp_m_in[y - 4][x + 3];
					window_data2[8] = temp_m_in[y - 4][x + 4];

					window_data2[9] = temp_m_in[y - 3][x - 4];
					window_data2[10] = temp_m_in[y - 3][x - 3];
					window_data2[11] = temp_m_in[y - 3][x - 2];
					window_data2[12] = temp_m_in[y - 3][x - 1];
					window_data2[13] = temp_m_in[y - 3][x];
					window_data2[14] = temp_m_in[y - 3][x + 1];
					window_data2[15] = temp_m_in[y - 3][x + 2];
					window_data2[16] = temp_m_in[y - 3][x + 3];
					window_data2[17] = temp_m_in[y - 3][x + 4];

					window_data2[18] = temp_m_in[y - 2][x - 4];
					window_data2[19] = temp_m_in[y - 2][x - 3];
					window_data2[20] = temp_m_in[y - 2][x - 2];
					window_data2[21] = temp_m_in[y - 2][x - 1];
					window_data2[22] = temp_m_in[y - 2][x];
					window_data2[23] = temp_m_in[y - 2][x + 1];
					window_data2[24] = temp_m_in[y - 2][x + 2];
					window_data2[25] = temp_m_in[y - 2][x + 3];
					window_data2[26] = temp_m_in[y - 2][x + 4];

					window_data2[27] = temp_m_in[y - 1][x - 4];
					window_data2[28] = temp_m_in[y - 1][x - 3];
					window_data2[29] = temp_m_in[y - 1][x - 2];
					window_data2[30] = temp_m_in[y - 1][x - 1];
					window_data2[31] = temp_m_in[y - 1][x];
					window_data2[32] = temp_m_in[y - 1][x + 1];
					window_data2[33] = temp_m_in[y - 1][x + 2];
					window_data2[34] = temp_m_in[y - 1][x + 3];
					window_data2[35] = temp_m_in[y - 1][x + 4];

					window_data2[36] = temp_m_in[y][x - 4];
					window_data2[37] = temp_m_in[y][x - 3];
					window_data2[38] = temp_m_in[y][x - 2];
					window_data2[39] = temp_m_in[y][x - 1];
					window_data2[40] = temp_m_in[y][x];
					window_data2[41] = temp_m_in[y][x + 1];
					window_data2[42] = temp_m_in[y][x + 2];
					window_data2[43] = temp_m_in[y][x + 3];
					window_data2[44] = temp_m_in[y][x + 4];

					window_data2[45] = temp_m_in[y + 1][x - 4];
					window_data2[46] = temp_m_in[y + 1][x - 3];
					window_data2[47] = temp_m_in[y + 1][x - 2];
					window_data2[48] = temp_m_in[y + 1][x - 1];
					window_data2[49] = temp_m_in[y + 1][x];
					window_data2[50] = temp_m_in[y + 1][x + 1];
					window_data2[51] = temp_m_in[y + 1][x + 2];
					window_data2[52] = temp_m_in[y + 1][x + 3];
					window_data2[53] = temp_m_in[y + 1][x + 4];

					window_data2[54] = temp_m_in[y + 2][x - 4];
					window_data2[55] = temp_m_in[y + 2][x - 3];
					window_data2[56] = temp_m_in[y + 2][x - 2];
					window_data2[57] = temp_m_in[y + 2][x - 1];
					window_data2[58] = temp_m_in[y + 2][x];
					window_data2[59] = temp_m_in[y + 2][x + 1];
					window_data2[60] = temp_m_in[y + 2][x + 2];
					window_data2[61] = temp_m_in[y + 2][x + 3];
					window_data2[62] = temp_m_in[y + 2][x + 4];

					window_data2[63] = temp_m_in[y + 3][x - 4];
					window_data2[64] = temp_m_in[y + 3][x - 3];
					window_data2[65] = temp_m_in[y + 3][x - 2];
					window_data2[66] = temp_m_in[y + 3][x - 1];
					window_data2[67] = temp_m_in[y + 3][x];
					window_data2[68] = temp_m_in[y + 3][x + 1];
					window_data2[69] = temp_m_in[y + 3][x + 2];
					window_data2[70] = temp_m_in[y + 3][x + 3];
					window_data2[71] = temp_m_in[y + 3][x + 4];

					window_data2[72] = temp_m_in[y + 4][x - 4];
					window_data2[73] = temp_m_in[y + 4][x - 3];
					window_data2[74] = temp_m_in[y + 4][x - 2];
					window_data2[75] = temp_m_in[y + 4][x - 1];
					window_data2[76] = temp_m_in[y + 4][x];
					window_data2[77] = temp_m_in[y + 4][x + 1];
					window_data2[78] = temp_m_in[y + 4][x + 2];
					window_data2[79] = temp_m_in[y + 4][x + 3];
					window_data2[80] = temp_m_in[y + 4][x + 4];

					index++;
					weight_table[index] = Gaussian_distance_weight(window_data1, window_data2, gaussian_core, SD, 9);
					//printf("\n dhfhf %d", index);
				}
			}


			total_weight = 0.0;
			for (int r = 0; r < 39 * 39; r++)
			{
				total_weight += weight_table[r];
			}
			//double test = 0.0;
			for (int k = 0; k < 39 * 39; k++)
			{
				adjusted_weight_table[k] = weight_table[k] / total_weight;
				//test += adjusted_weight_table[k];
			}
			filtered_value = 0.0;
			index = 0;
			for (int y1 = i - 23 + 4; y1 <= i + 23 - 4; y1++)
			{
				for (int x1 = j - 23 + 4; x1 <= j + 23 - 4; x1++)
				{
					filtered_value = filtered_value + (double)temp_m_in[y1][x1] * adjusted_weight_table[index];
					index++;
					//printf("\n test is %lf", filtered_value);
				}
			}

			output[width * (i - 23) + j - 23] = (int)filtered_value;

		}
	}

}





