#include "kernel.hpp"
#include "utils.hpp"

// const size_t data_size = 10;
const size_t data_size = 10000;
const uint32_t seed = 666;

int main(int argc, char const *argv[]) {
  std::vector<int32_t> v(data_size);

  int32_t min = -256;
  int32_t max = 256;

  initialize(v, min, max, seed);

  print_vector(v);

  sort(v, min, max);

  print_vector(v);

  return 0;
}
