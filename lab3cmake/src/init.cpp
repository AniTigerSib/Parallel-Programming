#include "diff_equation.h"

// Задание граничных значений
double G(double x, double y)
{
	if (x == 0) return 1 - 2 * y;
	if (x == 1) return -1 + 2 * y;
	if (y == 0) { return 1 - 2 * x; }
    else return -1 + 2 * x;
}

// Задание правой части
double F(double x, double y)
{
	return 2.2;
}

// Инициализация массивов правой части и краевых условий
void Init(double **u, double **f, int N)
{
	double h = 1.0 / (N + 1);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			f[i][j] = F((i + 1) * h, (j + 1) * h);
	}
	for (int i = 1; i < N + 1; i++)
	{
		for (int j = 1; j < N + 1; j++)
			u[i][j] = 0.2;
		u[i][0] = G(i * h, 0);
		u[i][N + 1] = G(i * h, (N + 1) * h);
	}
	for (int j = 0; j < N + 2; j++)
	{
		u[0][j] = G(0, j * h);
		u[N + 1][j] = G((N + 1) * h, j * h);
	}
}