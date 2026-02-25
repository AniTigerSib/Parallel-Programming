#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define sched static, 64
#define N 20000

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

void run_parallel_external() {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  #pragma omp parallel for private(i, j) schedule(sched)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_dynamic_external(int **matrix) {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  
  #pragma omp parallel for private(i, j) schedule(sched)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      matrix[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_internal() {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  for (i = 0; i < N; i++) {
    #pragma omp parallel for private(j) schedule(sched)
    for (j = 0; j < N; j++) {
      a[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_dynamic_internal(int **matrix) {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  
  for (i = 0; i < N; i++) {
    #pragma omp parallel for private(j) schedule(sched)
    for (j = 0; j < N; j++) {
      matrix[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_collapse() {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  #pragma omp parallel for private(i, j) schedule(sched) collapse(2)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_parallel_dynamic_collapse(int **matrix) {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  
  #pragma omp parallel for private(i, j) schedule(sched) collapse(2)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      matrix[i][j] = rand();
    }
  }
  
  timespec_get(&ts2, TIME_UTC);

  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

int main()
{
  srand(clock());
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

  printf("\nStatic parallel run - external parallel:\n");
  run_parallel_external();
  printf("Dynamic parallel run - external parallel:\n");
  run_parallel_dynamic_external(matrix);

  printf("\nStatic parallel run - internal parallel:\n");
  run_parallel_internal();
  printf("Dynamic parallel run - internal parallel:\n");
  run_parallel_dynamic_internal(matrix);

  printf("\nStatic parallel run - collapse parallel:\n");
  run_parallel_internal();
  printf("Dynamic parallel run - collapse parallel:\n");
  run_parallel_dynamic_internal(matrix);
  return 0;
}
