#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 40000

int a[N][N];

void warmup_run() {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      a[i][j] = 0;
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_serial() {
  int i, j, k = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      a[i][j] = k++;
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel() {
  int i, j, k = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  #pragma omp parallel for private(i, j) firstprivate(k)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = k++;
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

// DYNAMIC /////

void warmup_run_dynamic(int **matrix) {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      matrix[i][j] = 0;
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_serial_dynamic(int **matrix) {
  int i, j, k = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      matrix[i][j] = k++;
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_dynamic(int **matrix) {
  int i, j, k = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  
  #pragma omp parallel for private(i, j) firstprivate(k)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      matrix[i][j] = k++;
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

int main()
{
  printf("STATIC:\n");

  printf("Warm up:\n");
  warmup_run();
  printf("Serial run:\n");
  run_serial();
  
  int **matrix = (int **)calloc(sizeof(int *), N);
  for (int i = 0; i < N; i++) {
    matrix[i] = (int *)calloc(sizeof(int), N);
  }

  printf("DYNAMIC:\n");

  printf("Warm up:\n");
  warmup_run_dynamic(matrix);
  printf("Serial run:\n");
  run_serial_dynamic(matrix);

  int num_threads = 2;
  for (; num_threads <= 16; num_threads += 2) {
    omp_set_num_threads(num_threads);
    printf("\nThread num - %d\n", num_threads);
    printf("Static parallel run:\n");
    run_parallel();
    printf("Dynamic parallel run:\n");
    run_parallel_dynamic(matrix);
  }
  return 0;
}
