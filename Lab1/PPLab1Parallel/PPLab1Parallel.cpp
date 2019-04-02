// PPLab1Parallel.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "time.h"
#include <fstream>
#include "string"

using namespace std;

struct Args
{
	int size, row, col;
};

vector<vector<int>> result, baseMatrix;

int getDeterm(const vector<vector<int>> &matr, int minorSize) {
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

vector<vector<int>> getMinor(const Args &args) {
	int minorSize = args.size - 1, x = 0, y = 0;
	vector<vector<int>> minor(minorSize, vector<int>(minorSize));

	for (int i = 0; i < args.size; i++)
	{
		for (int j = 0; j < args.size; j++)
		{
			if ((i != args.row) && (j != args.col))
			{
				minor[x][y] = baseMatrix[i][j];
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
	int minorSize = args->size - 1, x = 0, y = 0, determ = 0;
	// обходим каждый элемент в строке
	vector<int> line(minorSize);
	for (int col = 0; col < args->size; col++) {
		args->col = col;
		vector<vector<int>> minor = getMinor(*args);
		determ = getDeterm(minor, minorSize);
		// расчитывает алг.доп. для элемента
		result[args->row][args->col] = ((args->row + args->col) % 2 == 0) ? determ : -determ;
	}

	return 0;
}

void getAdditions(int size, int countThreads) {
	HANDLE *handles = new HANDLE[countThreads];
	result = baseMatrix;
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
		minorArgs->size = size;
		minorArgs->row = row;
		// создаем поток
		handles[indexThread] = CreateThread(NULL, 0, ThreadFunc, (PVOID)minorArgs, 0, NULL);
		indexThread++;
	}
	WaitForMultipleObjects(countThreads, handles, TRUE, INFINITE);
}

void printMatrix(const vector<vector<int>>& matrix) {
	for (auto i : matrix) {
		for (int j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}

int readMatrix() {
	string inputFileName = "input.txt";
	ifstream input(inputFileName);

	if (!input.is_open()) {
		cerr << "Error open " << inputFileName << " file" << endl;
		exit(1);
	}

	int size = 0, value = 0, row = 0, col = 0;
	input >> size;
	vector<vector<int>> matrix(size, vector<int>(size));

	while (input >> value)
	{
		matrix[row][col] = value;
		col++;
		if (col % size == 0)
		{
			col = 0;
			row++;
		}
	}

	baseMatrix = matrix;

	return size;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "Not enough args! You must set count threads" << endl;
		return 1;
	}
	int value = 0, size = 0, countThreads = 2;
	if (countThreads < 1) {
		cout << "Count threads always greater then 1" << endl;
		return 1;
	}
	HANDLE process = GetCurrentProcess();
	SetProcessAffinityMask(process, 0b0001);
	size = readMatrix();
	int t = clock();
	getAdditions(size, countThreads);
	cout << clock() - t << " ms" << endl;
	// printMatrix(result);
}