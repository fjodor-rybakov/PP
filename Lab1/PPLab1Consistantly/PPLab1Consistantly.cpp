﻿// PPLab1Consistantly.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include "time.h"
#include <fstream>
#include "string"

using namespace std;

vector<vector<int>> baseMatrix;

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

int getMinor(int k, int l, const vector<vector<int>> &matrix, int size)
{
	int minorSize = size - 1, x = 0, y = 0;
	vector<vector<int>> minor(minorSize, vector<int>(minorSize));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((i != k) && (j != l))
			{
				minor[x][y] = matrix[i][j];
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

void printMatrix(const vector<vector<int>>& matrix) {
	for (auto i : matrix) {
		for (int j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}

vector<vector<int>> getAdditions(const vector<vector<int>>& matrix, int size) {
	vector<vector<int>> result(size, vector<int>(size));
	int determ = 0;
	for (int k = 0; k < size; k++)
	{
		for (int l = 0; l < size; l++)
		{
			determ = getMinor(k, l, matrix, size);
			result[k][l] = ((k + l) % 2 == 0) ? determ : -determ;
		}
	}

	return result;
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

int main()
{
	int value = 0, size = 0;
	size = readMatrix();
	vector<vector<int>> matrix;

	int t = clock();
	matrix = getAdditions(baseMatrix, size);
	cout << clock() - t << " ms" << endl;

	// printMatrix(matrix);
}