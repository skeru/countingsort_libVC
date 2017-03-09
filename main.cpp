#include "kernel.hpp"
#include "TimeMonitor.hpp"
#include "utils.hpp"

#include "versioningCompiler/Version.hpp"
#include "versioningCompiler/CompilerImpl/SystemCompiler.hpp"

#include <sched.h>

#ifndef _PATH_TO_KERNEL
#define PATH_TO_KERNEL "../"
#else
#define PATH_TO_KERNEL _PATH_TO_KERNEL
#endif

typedef void (*kernel_t)(std::vector<int32_t> &array);

// const size_t data_size = 10;
const size_t data_size = 1000000;
uint32_t seed = 666;
const size_t MAX_ITERATIONS = 1000;

void run_test(size_t data_size, int32_t min, int32_t max, size_t iterations);

int main(int argc, char const *argv[]) {

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  std::vector<int32_t> min = {-256, -512, -1024};
  std::vector<int32_t> max = {256, 512, 1024};

  for (size_t i = 0; i < min.size(); i++) {
    run_test(data_size, min[i], max[i], MAX_ITERATIONS);
  }

  return 0;
}


void run_test(size_t data_size, int32_t min, int32_t max, size_t iterations) {

  TimeMonitor time_monitor1;
  TimeMonitor time_monitor2;

  std::vector<int32_t> v(data_size);

  for (size_t i = 0; i < iterations; i++) {
    initialize(v, min, max, seed++);
    time_monitor1.start();
    sort(v, min, max);
    time_monitor1.stop();
  }

  vc::Version::Builder builder;
  const std::string kernel_dir = PATH_TO_KERNEL;
  builder._fileName_src = kernel_dir + "kernel.cpp";
  builder._compiler = std::make_shared<vc::SystemCompiler>();
  builder._functionName = "vc_sort";
  builder._optionList = {
    vc::Option("opt lv", "-O", "3"),
    vc::Option("std", "-std=", "c++11"),
    vc::Option("include dir", "-I", kernel_dir),
    vc::Option("min range", "-D_MIN_VALUE_RANGE=", std::to_string(min)),
    vc::Option("max range", "-D_MAX_VALUE_RANGE=", std::to_string(max)),
  };
  auto version = builder.build();
  if (! version->compile()) {
    std::cerr << "error compiling kernel version" << std::endl;
    return;
  }
  kernel_t f = (kernel_t) version->getSymbol();

  for (size_t i = 0; i < iterations; i++) {
    initialize(v, min, max, seed++);
    time_monitor2.start();
    f(v);
    time_monitor2.stop();
  }

  std::cout << "[ " << min << ", " << max << " ]\t Time taken 1 (avg) " << time_monitor1.getAvg() << " ms" << std::endl;
  std::cout << "[ " << min << ", " << max << " ]\t Time taken 2 (avg) " << time_monitor2.getAvg() << " ms" << std::endl;
  return;
}
