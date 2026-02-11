#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 40000

int a[N][N];

void run_zeroes() {
  int i, j = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);
  clock_t t_start = clock();
  
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = 0;
    }
  }

  timespec_get(&ts2, TIME_UTC);
  clock_t t_finish = clock();

  double duration = 1000.0 * (t_finish - t_start) / CLOCKS_PER_SEC;
  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per clock()): %.2f ms\n", duration);
  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_increment() {
  int i, j, k = 1;

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);
  clock_t t_start = clock();
  
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = k++;
    }
  }

  timespec_get(&ts2, TIME_UTC);
  clock_t t_finish = clock();

  double duration = 1000.0 * (t_finish - t_start) / CLOCKS_PER_SEC;
  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per clock()): %.2f ms\n", duration);
  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

void run_random() {
  int i, j = 1;
  srand(clock());

  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);
  clock_t t_start = clock();
  
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = rand();
    }
  }

  timespec_get(&ts2, TIME_UTC);
  clock_t t_finish = clock();

  double duration = 1000.0 * (t_finish - t_start) / CLOCKS_PER_SEC;
  double tspec_duration = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                        - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);

  printf("CPU time used (per clock()): %.2f ms\n", duration);
  printf("CPU time used (per timespec_get()): %.2f ms\n", tspec_duration);
}

int main()
{
  printf("Warm up:\n");
  run_zeroes();
  printf("\nRun zeroes:\n");
  run_zeroes();
  printf("\nRun increment:\n");
  run_increment();
  printf("\nRun random:\n");
  run_random();
  
  return 0;
}
