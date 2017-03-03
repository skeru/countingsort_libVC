#include "utils.hpp"

void findMinMax(const std::vector<int32_t> &arr, int32_t& min, int32_t& max ) {
  if (arr.size() > 0) {
    min = max = arr[0];
  }
  for( const auto &e : arr ) {
    if( e > max ) max = e;
    if( e < min ) min = e;
  }
  return;
}

void initialize(std::vector<int32_t> &v, int32_t min, int32_t max, uint32_t seed) {
  srand(seed);
  for (auto & elem : v) {
    elem = static_cast<int32_t>(rand() % (max - min)) + min;
  }
  return;
}
