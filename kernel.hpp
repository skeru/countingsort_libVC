#ifndef COUNTING_SORT_LIBVC_KERNEL_HPP
#define COUNTING_SORT_LIBVC_KERNEL_HPP

#include <cstdint>
#include <vector>
#include "utils.hpp"


void sort(std::vector<int32_t> &array, const int32_t min, const int32_t max);

extern "C"
void vc_sort(std::vector<int32_t> &array, const int32_t min, const int32_t max);

#endif /* end of include guard: COUNTING_SORT_LIBVC_KERNEL_HPP */
