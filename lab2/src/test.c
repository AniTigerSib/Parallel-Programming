#include <stdio.h>

int main() {
  #ifdef _OPENMP
  printf("%d", _OPENMP);
  #else
  printf("Not found");
  #endif
  return 0;
}