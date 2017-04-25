#ifndef UTILS_TIME_MONITOR_HPP
#define UTILS_TIME_MONITOR_HPP

#include <chrono>
#include <cstdint>

typedef std::chrono::high_resolution_clock::time_point time_acquisition_t;
typedef std::chrono::duration<double, std::milli> time_accumulation_t;

class TimeMonitor {

public:
  TimeMonitor();

  void start();

  double stop();

  double getLast();

  double getAvg();

  double getMax();

private:
  uint64_t _counter;
  time_acquisition_t _start;
  time_acquisition_t _stop;
  time_accumulation_t _last_run_time;
  double _sum;
  double _max;

};

#endif /* end of include guard: UTILS_TIME_MONITOR_HPP */
