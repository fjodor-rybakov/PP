// PPLab1Consistantly.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include "time.h"

using namespace std;

const int n = 3;
int matrix[n][n] = { 1, -2, 0, 4, 6, 2, -3, 4, -2 };

int getMinor(int k, int l) {
	vector<int> minor;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if ((i != k) && (j != l))
				minor.push_back(matrix[i][j]);

	return minor[0] * minor[3] - minor[1] * minor[2];
}

void getAdditions() {
	for (int k = 0; k < n; k++) {
		for (int l = 0; l < n; l++) {
			if ((k + l) % 2 == 0) {
				cout << getMinor(k, l) << " ";
			}
			else
			{
				cout << -getMinor(k, l) << " ";
			}
		}

		cout << endl;
	}
}

int main()
{
	/*int value;
	cout << "Enter pls matrix 3x3" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> value;
			matrix[i][j] = value;
		}
	}*/
	int t = clock();

	getAdditions();

	cout << clock() - t << " ms" << endl;
}