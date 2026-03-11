#include "diff_equation.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Последовательная функция, реализующая алгоритм Гаусса-Зейделя
// Входные параметры: массив неизвестных и краевых значений, массив правых частей, количество точек сетки по каждому направлению, точность вычислений
int Calc_ser(double** u, double** f, int N, double eps)
{
	double max;                // Максимальная ошибка на итерации
	double h = 1.0 / (N + 1);  // Величина шага
	int icnt = 0;              // Количество итераций

	do
	{
		icnt++;
		max = 0;
		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= N; j++)
			{
				double u0 = u[i][j];
				u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j]     // Расчет по формуле Гаусса-Зейделя
					         + u[i][j - 1] + u[i][j + 1] - h * h * f[i - 1][j - 1]);
				double d = fabs(u[i][j] - u0);      // Разность нового значения неизвестной и значения с предыдущей итерации
				if (d > max)                       // Поиск максимальной ошибки
					max = d;
			}
	}
	while (max > eps);

	return icnt;
}

// Последовательная функция, реализующая блочный алгоритм Гаусса-Зейделя
int Calc_blk(double** u, double** f, int N, double eps, const int BlockSize)
{
	double max;
	double h = 1.0 / (N + 1);
	int icnt = 0;

    
	if (N % BlockSize != 0) // Если количество точек по каждому из направлений сетки делится нацело на размер блока, то проводятся вычисления
    {
        cout << "Error!!!" << endl;
        return icnt;
    }
    int bcnt = N / BlockSize; // Количество блоков в ряд
    do
    {
        icnt++;
        max = 0;
        
        for (int bl_row = 0; bl_row < bcnt; bl_row++) {
            for (int bl_col = 0; bl_col < bcnt; bl_col++) {
                for (int i = 1 + bl_row * BlockSize; i <= (bl_row + 1) * BlockSize; i++) {
                    for (int j = 1 + bl_col * BlockSize; j <= (bl_col + 1) * BlockSize; j++) {
                        double u0 = u[i][j];
                        u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j]     // Расчет по формуле Гаусса-Зейделя
                                + u[i][j - 1] + u[i][j + 1] - h * h * f[i - 1][j - 1]);
                        double d = fabs(u[i][j] - u0);      // Разность нового значения неизвестной и значения с предыдущей итерации
                        if (d > max)                       // Поиск максимальной ошибки
                            max = d;
                    }
                }
            }
        }
    }
    while (max > eps);
	
	return icnt;
}

// Параллельная реализия блочного алгоритма Гаусса-Зейделя
int Calc_par(double** u, double** f, int N, double eps, const int BlockSize)
{
	double max;
	const double h = 1.0 / (N + 1);
	int icnt = 0;

    if (N % BlockSize != 0)
    {
        cout << "Error!!!" << endl;
        return icnt;
    }

	const int bcnt = N / BlockSize;
    vector<vector<pair<int, int>>> diag_blocks(2 * bcnt - 1);
    for (int diag = 0; diag < 2 * bcnt - 1; diag++) {
        for (int i = 0; i <= diag; i++) {
            int bl_row = diag - i;
            int bl_col = i;
            if (bl_row < bcnt && bl_col < bcnt) {
                diag_blocks[diag].emplace_back(bl_row, bl_col);
            }
        }
    }

	do
	{
		icnt++;
		max = 0.0;
		
        {
            for (int diag = 0; diag < 2 * bcnt - 1; diag++) {
                #pragma omp parallel for shared(u, f, diag, diag_blocks) schedule(guided) reduction(max:max)
                for (int block_idx = 0; block_idx < diag_blocks[diag].size(); block_idx++) {
                    auto [bl_row, bl_col] = diag_blocks[diag][block_idx];
                    for (int i = 1 + bl_row * BlockSize; i <= (bl_row + 1) * BlockSize; i++) {
                        for (int j = 1 + bl_col * BlockSize; j <= (bl_col + 1) * BlockSize; j++) {
                            double u0 = u[i][j];
                            u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j]     // Расчет по формуле Гаусса-Зейделя
                                    + u[i][j - 1] + u[i][j + 1] - h * h * f[i - 1][j - 1]);
                            double d = fabs(u[i][j] - u0);      // Разность нового значения неизвестной и значения с предыдущей итерации
                            if (d > max)
                                max = d;
                        }
                    }
                }
                #pragma omp barrier
            }
        }
        // {
        //     for (int diag = 0; diag < bcnt; diag++) {
        //         #pragma omp for schedule(guided) reduction(max:max)
        //         for (int i = 0; i < diag; i++) {
        //             const int min_row = 1 + (diag - i) * BlockSize;
        //             const int min_col = 1 + i * BlockSize;
        //             const int max_row = min_row + BlockSize - 1;
        //             const int max_col = min_col + BlockSize - 1;
        //             for (int row = min_row; row <= max_row; row++) {
        //                 for (int col = min_col; col < max_col; col++) {
        //                     double u0 = u[row][col];
        //                     u[row][col] = 0.25 * (u[row - 1][col] + u[row + 1][col]     // Расчет по формуле Гаусса-Зейделя
        //                             + u[row][col - 1] + u[row][col + 1] - h * h * f[row - 1][col - 1]);
        //                     double d = fabs(u[row][col] - u0); // Разность нового значения неизвестной и значения с предыдущей итерации
        //                     if (d > max)
        //                         max = d;
        //                 }
        //             }
        //         }
        //     }

        //     for (int diag = 0; diag < bcnt - 1; diag++) {
        //         #pragma omp for schedule(guided) reduction(max:max)
        //         for (int i = 0; i < diag; i++) {
        //             const int min_row = 1 + (bcnt - 1 - i) * BlockSize;
        //             const int min_col = 1 + (i + 1) * BlockSize;
        //             const int max_row = min_row + BlockSize - 1;
        //             const int max_col = min_col + BlockSize - 1;
        //             for (int row = min_row; row <= max_row; row++) {
        //                 for (int col = min_col; col < max_col; col++) {
        //                     double u0 = u[row][col];
        //                     u[row][col] = 0.25 * (u[row - 1][col] + u[row + 1][col] // Расчет по формуле Гаусса-Зейделя
        //                             + u[row][col - 1] + u[row][col + 1] - h * h * f[row - 1][col - 1]);
        //                     double d = fabs(u[row][col] - u0); // Разность нового значения неизвестной и значения с предыдущей итерации
        //                     if (d > max)
        //                         max = d;
        //                 }
        //             }
        //         }
        //     }
        // }
	}
    while (max > eps);
	
	return icnt;
}