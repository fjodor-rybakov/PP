// PPLab1Parallel.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "time.h"

using namespace std;

struct Args
{
	int k;
	int l;
	vector<vector<int>> matrix;
	int size;
};

int getDeterm(vector<vector<int>> matr, int minorSize) {
	int sum, mul;
	sum = 0;
	if (2 == minorSize)
	{
		sum = matr[0][0] * matr[1][1] - matr[0][1] * matr[1][0];
		return sum;
	}
	for (int i = 0; i < minorSize; i++)
	{
		mul = 1;
		int k = i;
		for (int j = 0; j < minorSize; j++)
		{
			mul *= matr[j][k];
			k = k == minorSize - 1 ? 0 : k + 1;
		}
		sum += mul;
	}
	for (int i = 0; i < minorSize; i++)
	{
		mul = 1;
		int k = i;
		for (int j = 0; j < minorSize; j++)
		{
			mul *= matr[j][k];
			k = k == 0 ? minorSize - 1 : k - 1;
		}
		sum -= mul;
	}
	return sum;
}


DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	Args args = *((Args *)pvParam);

	int minorSize = args.size - 1, x = 0, y = 0;
	vector<vector<int>> minor(minorSize, vector<int>(minorSize));

	for (int i = 0; i < args.size; i++)
	{
		for (int j = 0; j < args.size; j++)
		{
			if ((i != args.k) && (j != args.l))
			{
				minor[x][y] = args.matrix[i][j];
				y++;
				if (y % minorSize == 0)
				{
					x++;
					y = 0;
				}
			}
		}
	}

	return getDeterm(minor, minorSize);
}

void getAdditions(vector<vector<int>> matrix, int size) {
	HANDLE hThread;
	for (int k = 0; k < size; k++)
	{
		for (int l = 0; l < size; l++)
		{
			Args minorArgs;
			minorArgs.k = k;
			minorArgs.l = l;
			minorArgs.matrix = matrix;
			minorArgs.size = size;
			hThread = CreateThread(NULL, 0, ThreadFunc, (PVOID)&minorArgs, 0, NULL);

			if ((k + l) % 2 == 0) {
				cout << getMinor(minorArgs) << " ";
			}
			else
			{
				cout << -getMinor(minorArgs) << " ";
			}
		}

		cout << endl;
	}
}

int main()
{
	int value = 0, size = 0;
	cout << "Enter pls size matrix" << endl;
	cin >> size;
	vector<vector<int>> matrix(size, vector<int>(size));
	cout << "Enter pls matrix " << size << "x" << size << endl;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cin >> value;
			matrix[i][j] = value;
		}
	}

	int t = clock();
	getAdditions(matrix, size);
	cout << clock() - t << " ms" << endl;
}