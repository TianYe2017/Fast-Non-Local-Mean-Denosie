//Date:February 3,2017
//Name:Tian Ye
//ID:2581-2031-06
//Email:tye459@usc.edu

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "border.h"


//�������鳤�ȴ���1000000��ͼƬ�����������1000�����޸���ʱ��������С����������ͼƬ
//����һ��
void Border_extend(int original_array[], int out_array[], int width, int height)  
{   
	int B[1000][1000];
	int C[1000][1000];
	int A[1000 * 1000];
	int Aout[1000*1000] = { 0 };
	int Aout2[1000*1000] = { 0 };

    //������A[i]
	for (int i = 0; i < width * height; i++) 
	{
		A[i] = original_array[i];
	}
	//ȫ������һ��
	for (int i = 0; i < width * height; i++)
	{
		Aout[i + width] = A[i];
	}  
	//�������һ��
	for (int i = 0; i < width; i++)
	{
		Aout[i] = Aout[i + width];
	}
	//�����һ��
	for (int i = 0; i < width; i++)
	{
		Aout[width * height + width + i] = Aout[width*height + i];
	}

	//ת��Ϊ����
	for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			B[i][j] = Aout[width * i + j];
		}
	}
	//����ת��
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			C[i][j] = B[j][i];
		}
	}
	//ת��������
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			Aout[(height + 2)* i + j] = C[i][j];
		}
	}

	//������ȫ������һ��
	for (int i = 0; i < width * (height + 2); i++)
	{
		Aout2[i + (height + 2)] = Aout[i];
	}
    //�������һ��
	for (int i = 0; i < (height + 2); i++)
	{
		Aout2[(width + 1) * (height + 2) + i] = Aout2[width * (height + 2) + i];
	}
	//�����һ��
	for (int i = 0; i < height + 2; i++)
	{
		Aout2[i] = Aout2[height + 2 + i];
	}

	//ת��Ϊ����
	for (int i = 0; i < width + 2; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			B[i][j] = Aout2[(height + 2) * i + j];
		}
	}
	//����ת��
    for (int i = 0; i < width + 2; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			C[j][i] = B[i][j];
		}
	}
	//ת��������
	for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			Aout[(width + 2) * i + j] = C[i][j];
		}
	}
	//�����Aout
	for (int i = 0; i < (width + 2)*(height + 2);i++)
	{
		out_array[i] = Aout[i];
	} 
}
