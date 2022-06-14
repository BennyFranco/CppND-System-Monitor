#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto activeJiffies = LinuxParser::ActiveJiffies(pid);
  auto systemJiffies = LinuxParser::Jiffies();

  auto totalActive = static_cast<float>(activeJiffies - activeJiffiesCache);
  auto totalSystem = static_cast<float>(systemJiffies - systemJiffiesCache);

  long startTime = LinuxParser::GetProcStatValue(LinuxParser::kStartTime, pid);
  long seconds = LinuxParser::UpTime() - (startTime / sysconf(_SC_CLK_TCK));

  float cpuUsage = (totalActive / totalSystem) / static_cast<float>(seconds);

  activeJiffiesCache = activeJiffies;
  systemJiffiesCache = systemJiffies;

  return cpuUsage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process a) {
  return CpuUtilization() < a.CpuUtilization();
}
Process::Process(int pid) : pid(pid) {}
