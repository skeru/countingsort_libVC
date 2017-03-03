#ifndef COUNTING_SORT_LIBVC_UTILS_HPP
#define COUNTING_SORT_LIBVC_UTILS_HPP

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

void findMinMax(const std::vector<int32_t> &arr, int32_t& min, int32_t& max );

template <typename my_t>
void print_vector(const std::vector<my_t> &v) {
  for (const auto &e : v) {
    std::cout << e << "\t";
  }
  std::cout << std::endl;
  return;
}

void initialize(std::vector<int32_t> &v, int32_t min, int32_t max, uint32_t seed);

#endif /* end of include guard: COUNTING_SORT_LIBVC_UTILS_HPP */
