#ifndef WORKLOAD_HPP
#define WORKLOAD_HPP

#include <cstdint>
#include <cstddef>
#include <vector>

template <typename data_t>
struct Metadata {
public:
  size_t size;
  data_t minVal;
  data_t maxVal;
};

template <typename data_t>
struct Workload {
  Metadata<data_t> meta;
  std::vector<data_t> data;
public:
  size_t size() {return meta.size;}
  Metadata<data_t> getMetadata() {return meta;}
  std::vector<data_t>& getData() {return &data;}
};

template <typename data_t>
class WorkloadProducer {
public:
  static Workload<data_t> get_WL_with_bounds_size(data_t min, data_t max, size_t size, size_t seed = 0);
  static Workload<data_t> get_WL_with_bounds(data_t min, data_t max, size_t seed = 0);
  static Workload<data_t> get_WL_with_size(size_t size, size_t seed = 0);
};

// template<>
// class WorkloadProducer<float>;

// template<>
// class WorkloadProducer<int32_t>;

#include "Workload_impl.hpp"

#endif /* end of include guard: WORKLOAD_HPP */
