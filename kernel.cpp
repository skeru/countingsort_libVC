#include "kernel.hpp"

#include <algorithm>

#ifndef _MIN_VALUE_RANGE
#define _MIN_VALUE_RANGE 0
#endif

#ifndef _MAX_VALUE_RANGE
#define _MAX_VALUE_RANGE 10
#endif

namespace {

const size_t range_size = _MAX_VALUE_RANGE - _MIN_VALUE_RANGE;

std::vector<size_t> counter(range_size);

}

void sort(std::vector<int32_t> &array, const int32_t min, const int32_t max) {
  const size_t my_range_size = max - min;
  std::vector<size_t> my_counter(my_range_size);
  std::fill(my_counter.begin(), my_counter.end(), 0);

  for( size_t i = 0; i < array.size(); i++ ) {
    my_counter[array[i] - min]++;
  }

  auto it = array.begin();
  for( int32_t i = min; i < max; i++ ) {
    const size_t increment = my_counter[i - min];
    std::fill_n(it, increment, i);
    std::advance(it, increment);
  }

  return;
}

extern "C"
void vc_sort(std::vector<int32_t> &array) {
  const int32_t max = _MAX_VALUE_RANGE;
  const int32_t min = _MIN_VALUE_RANGE;

  std::fill(counter.begin(), counter.end(), 0);
  for( size_t i = 0; i < array.size(); i++ ) {
    counter[array[i] - min]++;
  }

  auto it = array.begin();
  for( size_t i = 0; i < range_size; i++ ) {
    const int32_t index = i + min;
    const size_t increment = counter[i];
    std::fill_n(it, increment, index);
    std::advance(it, increment);
  }

  return;
}
