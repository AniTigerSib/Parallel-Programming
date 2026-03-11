//------------------------------------------------------------
// Программа решения уравнений Пуассона методом Гаусса-Зейделя
//------------------------------------------------------------
#include "diff_equation.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>
#include "locale.h"

using namespace std;

int main(int argc, char **argv)
{
	double **u = nullptr, **f = nullptr,
		   **u2 = nullptr, **f2 = nullptr,
		   **u3 = nullptr, **f3 = nullptr;
	
	const int N = 1000;        // Количество точек сетки по каждой размерности
	const double eps = 0.0001;   // Точность вычислений
	int icnt;                  // Количество итераций

	const int blockSize = 100;
	
	f = new_arr(N);      // Выделение памяти под правую часть значений уравнения
	u = new_arr(N + 2);  // Выделение памяти под неизвестные и краевые условия

	//  Последовательная реализация

	cout << "\n\t*** Serial version ***\n";
	Init(u, f, N);                  // Инициализация краевых условий и правой части уравнения
	auto start = chrono::high_resolution_clock::now();
	icnt = Calc_ser(u, f, N, eps);  // Вызов функции расчета по методу Гаусса-Зейделя
	auto end = chrono::high_resolution_clock::now();
	cout << std::format("Solution time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << endl;
    cout << "Iterations =    " << icnt << endl;
	cout << "Results:\n";
	Output(u, N);                   // Вывод результатов на экран

	//	 Последовательная блочная реализация
	f2 = new_arr(N);
	u2 = new_arr(N + 2);

	cout << "\n\t*** Block serial version ***\n";
	Init(u2, f2, N);                  // Инициализация краевых условий и правой части
	start = chrono::high_resolution_clock::now();
	icnt = Calc_blk(u2, f2, N, eps, blockSize);  // Вызов блочной функции расчета
	end = chrono::high_resolution_clock::now();
	cout << std::format("Solution time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << endl;
    cout << "Iterations =    " << icnt << endl;
	cout << "Results:\n";
	Output(u2, N);

	compare(u, u2, N + 2);

	//   Параллельная реализация (блочная)
	for (int i = 2; i <= 24; i += 2) {
		cout << "\n#### THREADS NUM: " << i << " ####\n" << endl;
		f3 = new_arr(N);
		u3 = new_arr(N + 2);

		omp_set_num_threads(8);
		cout << "\n\t*** Parallel version ***\n";
		Init(u3, f3, N);                  // Инициализация краевых условий и правой части
		start = chrono::high_resolution_clock::now();
		icnt = Calc_par(u3, f3, N, eps, blockSize);  // Вызов параллельной функции расчета
		end = chrono::high_resolution_clock::now();
		cout << std::format("Solution time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << endl;
		cout << "Iterations =    " << icnt << endl;
		cout << "Results:\n";
		Output(u3, N);

		compare(u, u3, N + 2);

		delete_arr(f3, N);
  		delete_arr(u3, N + 2);
	}
	

  	// Освобождение памяти массивов
  	delete_arr(f, N);
  	delete_arr(u, N + 2);
	delete_arr(f2, N);
  	delete_arr(u2, N + 2);
  
	return 0;
}

// Функция выделения памяти под 2D массив
double** new_arr(int N)
{
	double** f = new double* [N];
	for (int i = 0; i < N; i++)
	{
		f[i] = new double [N];
	}
	return f;
}

// Функция освобождения памяти 2D массива
void delete_arr(double** arr, int N)
{
	for (int i = 0; i < N; i++)
	{
		delete[] arr[i];
	}
	delete[] arr;
}

// Функция вывода прореженной матрицы решения
void Output(double** u, int N)
{
  	const int K = 5;
	cout << fixed << setprecision(8);
	for (int i = 0; i <= K; i++)
	{
		for (int j = 0; j <= K; j++)
			cout << setw(12) << u[i * (N + 1) / K][j * (N + 1) / K];
		cout << endl;
	}
}

void compare(double** first, double** second, const int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (first[i][j] - second[i][j] > 1e-6) {
				cout << format("Matrices are different in [{},{}]", i, j) << endl;
				return;
			}
		}
	}
}