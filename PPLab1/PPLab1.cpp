// PPLab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>

const int n = 3;
int a[n][n] = { 1, -2, 0, 4, 6, 2, -3, 4, -2 };
int b[n][n] = { 0, 2, 0, 1, 1, 1, 5, -3, 10 };
int c[n][n];

DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	int num, i, j;
	num = (*((int *)pvParam));
	for (i = 0; i < n; i++)
	{
		c[num][i] = 0;
		for (j = 0; j < n; j++)
			c[num][i] += a[num][j] * b[j][i];
	}

	DWORD dwResult = 0;
	return dwResult;
}

int main()
{
	int i, j;
	int x[n];

	DWORD dwThreadId[n - 1], dw;
	HANDLE hThread[n - 1];

	for (i = 1; i < n; i++)
	{
		x[i] = i;
		hThread[i - 1] = CreateThread(NULL, 0, ThreadFunc, (PVOID)&x[i], 0, &dwThreadId[i - 1]);
		if (!hThread) printf("main process: thread %d not execute!", i);
	}
	
	x[0] = 0;
	ThreadFunc((PVOID)&x[0]);

	dw = WaitForMultipleObjects(n - 1, hThread, TRUE, INFINITE);

	for (i = 0; i < n; i++)
	{
		fprintf(stdout, "\n");
		for (j = 0; j < n; j++) printf("%d ", c[i][j]);
	}

	return 0;
}
