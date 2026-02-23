#include <iostream>

int main() {
  #ifdef _OPENMP
  std::cout << "OPENMP ver: " << _OPENMP << std::endl;
  #else
  std::cout << "Not found";
  #endif
  return 0;
}