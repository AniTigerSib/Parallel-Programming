#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstddef>
#include <vector>

typedef std::vector<double> custom_vector;

static const size_t kVectorSize = 4000;
static const size_t kIterations = 100;

custom_vector get_filled_vector(size_t n);
std::vector<custom_vector> get_filled_matrix(size_t n);

#endif