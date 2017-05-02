#include "kernel.hpp"
#include "TimeMonitor.hpp"
#include "utils.hpp"
#include "Workload.hpp"

#include "versioningCompiler/Utils.hpp"

#include <sched.h>
#include <map>

#ifndef _PATH_TO_KERNEL
#define PATH_TO_KERNEL "../"
#else
#define PATH_TO_KERNEL _PATH_TO_KERNEL
#endif

typedef void (*kernel_t)(std::vector<int32_t> &array, const int32_t, const int32_t);
std::map<size_t, std::shared_ptr<vc::Version> > memo;

uint32_t seed = 666;
const size_t MAX_ITERATIONS = 100;
const float similarity_ratio_recompilation_threshold = 2.0f;

void run_test(size_t data_size, size_t iterations, std::pair<int, int> range);

int main(int argc, char const *argv[]) {

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  const std::vector<std::pair<int, int> > data_range = {
    std::make_pair<int,int>(0,256),
    std::make_pair<int,int>(0,512),
    std::make_pair<int,int>(0,1024),
    std::make_pair<int,int>(0,2048),
    std::make_pair<int,int>(0,4096),
    std::make_pair<int,int>(0,8192),
    std::make_pair<int,int>(0,16384),
    std::make_pair<int,int>(0,32768),
  };

  const std::vector<size_t> data_size = {
                                        10*1000*1000,
                                       100*1000*1000,
                                      1000*1000*1000,
                                   10*1000*1000*1000,
                                  100*1000*1000*1000,
                                        };

  vc::vc_utils_init();

  for (const size_t s : data_size) {
    for (const auto r : data_range) {
      run_test(s, MAX_ITERATIONS, r);
    }
  }

  return 0;
}

// dynamically create a new version
std::shared_ptr<vc::Version> dynamicCompile(int32_t min, int32_t max) {
  const std::string kernel_dir = PATH_TO_KERNEL;
  const std::string kernel_file = kernel_dir + "kernel.cpp";
  const std::string functionName = "vc_sort";
  const std::list<vc::Option> opt_list = {
    vc::Option("opt lv", "-O", "3"),
    vc::Option("std", "-std=", "c++11"),
    vc::Option("include dir", "-I", kernel_dir),
    vc::Option("min range", "-D_MIN_VALUE_RANGE=", std::to_string(min)),
    vc::Option("max range", "-D_MAX_VALUE_RANGE=", std::to_string(max)),
  };
  std::shared_ptr<vc::Version> version = vc::createVersion(kernel_file, functionName, opt_list);
  // compiling...
  kernel_t f = (kernel_t) vc::compileAndGetSymbol(version);
  if (f) {
    return version;
  }
  return nullptr;
}

// lookup in previously compiled versions
std::shared_ptr<vc::Version> getDynamicVersion(int32_t min, int32_t max) {
  const size_t range = max - min;
  auto elem = memo.upper_bound(range);
  size_t threshold = range * similarity_ratio_recompilation_threshold;
  if (elem != memo.end() && elem->first < threshold) {
    elem->second->compile();
    return elem->second;
  }
  auto v = dynamicCompile(min, max);
  if (v != nullptr) {
    memo[range] = v;
  }
  return v;
}

void run_test(size_t data_size, size_t iterations, std::pair<int, int> range) {

  TimeMonitor time_monitor_ref;
  TimeMonitor time_monitor_dyn;
  TimeMonitor time_monitor_ovh;

  // running reference version - statically linked to main program
  for (size_t i = 0; i < iterations; i++) {
    auto wl = WorkloadProducer<int32_t>::get_WL_with_bounds_size(range.first,
                                                                 range.second,
                                                                 data_size,
                                                                 seed);
    const auto meta = wl.getMetadata();
    time_monitor_ref.start();
    sort(wl.data, meta.minVal, meta.maxVal);
    time_monitor_ref.stop();
  }

  // running dynamic version - dynamically compiled
  for (size_t i = 0; i < iterations; i++) {
    auto wl = WorkloadProducer<int32_t>::get_WL_with_bounds_size(range.first,
                                                                 range.second,
                                                                 data_size,
                                                                 seed);
    const auto meta = wl.getMetadata();
    time_monitor_ovh.start();
    auto v = getDynamicVersion(meta.minVal, meta.maxVal);
    kernel_t my_sort = (kernel_t) v->getSymbol();
    time_monitor_ovh.stop();
    if (! my_sort) {
      std::cerr << "Error while processing item " << i << std::endl;
      return;
    }
    time_monitor_dyn.start();
    my_sort(wl.data, meta.minVal, meta.maxVal);
    v->fold();
    time_monitor_dyn.stop();
  }

  std::cout << range.second << "\t" << data_size << "\t Avg Time taken ref " << time_monitor_ref.getAvg() << " ms" << std::endl;
  std::cout << range.second << "\t" << data_size << "\t Avg Time taken dyn " << time_monitor_dyn.getAvg() << " ms" << std::endl;
  std::cout << range.second << "\t" << data_size << "\t Max Time taken ovh " << time_monitor_ovh.getMax() << " ms" << std::endl;
  std::cout << range.second << "\t" << data_size << "\t Avg Time taken ovh " << time_monitor_ovh.getAvg() << " ms" << std::endl;
  return;
}
