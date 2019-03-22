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
	int size, row, col;
	vector<vector<int>> matrix, result;
};

struct ResultMatrix
{
	vector<vector<int>> result;
};

ResultMatrix *res = new ResultMatrix;

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

vector<vector<int>> getMinor(Args args) {
	int minorSize = args.size - 1, x = 0, y = 0;
	vector<vector<int>> minor(minorSize, vector<int>(minorSize));

	for (int i = 0; i < args.size; i++)
	{
		for (int j = 0; j < args.size; j++)
		{
			if ((i != args.row) && (j != args.col))
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

	return minor;
}

DWORD WINAPI ThreadFunc(const PVOID pvParam)
{
	Args *args = (Args *)pvParam;

	cout << "im here" << endl;
	//cout << args->row << endl;
	int minorSize = args->size - 1, x = 0, y = 0;
	// обходим каждый элемент в строке
	vector<int> line(minorSize);
	for (int col = 0; col < args->size; col++) {
		args->col = col;
		vector<vector<int>> minor = getMinor(*args);
		res->result[args->row][args->col] = ((args->row + args->col) % 2 == 0) ? getDeterm(minor, minorSize) : -getDeterm(minor, minorSize);
		cout << args->row << "  " << args->col << endl;
		// cout << (((args->row + args->col) % 2 == 0) ? getDeterm(minor, minorSize) : -getDeterm(minor, minorSize)) << endl;
	}

	return 0;
}

void getAdditions(vector<vector<int>>& matrix, int size, int countThreads) {
	HANDLE *handles = new HANDLE[countThreads];
	vector<vector<int>> resultMatrix = matrix;
	res->result = matrix;
	int indexThread = 0;
	// обходим столбцы
	for (int row = 0; row < size; row++)
	{
		if (indexThread == countThreads) {
			// ожидаем
			WaitForMultipleObjects(countThreads, handles, TRUE, INFINITE);
			indexThread = 0;
		}
		Args *minorArgs = new Args;
		minorArgs->matrix = matrix;
		minorArgs->size = size;
		minorArgs->result = matrix;
		minorArgs->row = row;
		// создаем поток
		handles[indexThread] = CreateThread(NULL, 0, ThreadFunc, (PVOID)minorArgs, 0, NULL);
		indexThread++;
	}
	WaitForMultipleObjects(countThreads, handles, TRUE, INFINITE);
}

int main(int argc, char* argv[])
{
	/*if (argc != 2) {
		cout << "Not enough args! You must set count threads" << endl;
		return 1;
	}*/
	int value = 0, size = 0, countThreads = 2;
	if (countThreads < 1) {
		cout << "Count threads always greater then 1" << endl;
		return 1;
	}
	if (countThreads > 4) {
		countThreads = 2;
	}
	cout << "Enter pls size matrix" << endl;
	// cin >> size;
	size = 3;
	vector<vector<int>> matrix(size, vector<int>(size));
	cout << "Enter pls matrix " << size << "x" << size << endl;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// cin >> value;
			matrix[i][j] = 4;
		}
	}
	int t = clock();
	getAdditions(matrix, size, countThreads);
	cout << clock() - t << " ms" << endl;
	for (auto i : res->result) {
		for (int j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}