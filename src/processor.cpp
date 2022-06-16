#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto idle = LinuxParser::IdleJiffies();
  auto nonIdle = LinuxParser::ActiveJiffies();

  auto prevTotal = prevIdle + prevNonIdle;
  auto total = idle + nonIdle;

  auto totald = static_cast<float>(total - prevTotal);
  auto idled = static_cast<float>(idle - prevIdle);

  prevIdle = idle;
  prevNonIdle = nonIdle;

  return (totald - idled) / totald;
}