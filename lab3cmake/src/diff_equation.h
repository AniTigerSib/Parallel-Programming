#ifndef DIFF_EQUATION_H
#define DIFF_EQUATION_H

// ‘ункции решени€ уравнени€ (результаты всех версий должны быть идентичны!)
int Calc_ser(double** u, double** f, int N, double eps);  // последовательна€
int Calc_blk(double** u, double** f, int N, double eps, const int BlockSize);  // блочна€ последовательна€
int Calc_par(double** u, double** f, int N, double eps, const int BlockSize);  // параллельна€ (блочна€)

// »нициализаци€ массивов
void Init(double **u, double **f, int N);
double** new_arr(int N);
void delete_arr(double** arr, int N);

// ¬ывод части массива дл€ контрол€
void Output(double** u, int N);

void compare(double** first, double** second, const int N);

#endif