#include "TimeMonitor.hpp"

TimeMonitor::TimeMonitor() {
  _counter = 0;
  _sum = 0;
}

void TimeMonitor::start() {
  _start = std::chrono::high_resolution_clock::now();
  return;
}

double TimeMonitor::stop() {
  _stop = std::chrono::high_resolution_clock::now();
  _counter ++;
  _last_run_time = _stop - _start;
  _sum += _last_run_time.count();
  return _last_run_time.count();
}

double TimeMonitor::getLast() {
  return _last_run_time.count();
}

double TimeMonitor::getAvg() {
  if (! _counter) return 0;
  return _sum / _counter;
}
