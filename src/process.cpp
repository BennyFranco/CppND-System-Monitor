#include "process.h"

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
  if (!LinuxParser::Exists(pid)) return cpuUsage;
  auto activeJiffies = LinuxParser::ActiveJiffies(pid);
  auto systemJiffies = LinuxParser::Jiffies();

  auto totalActive = static_cast<float>(activeJiffies - activeJiffiesCache);
  auto totalSystem = static_cast<float>(systemJiffies - systemJiffiesCache);

  activeJiffiesCache = activeJiffies;
  systemJiffiesCache = systemJiffies;

  cpuUsage = (totalActive / totalSystem);
  return cpuUsage;
}

/// DONE: Return the command that generated this process
string Process::Command() {
  if (LinuxParser::Exists(pid)) command = LinuxParser::Command(pid);
  return command;
}

// DONE: Return this process's memory utilization
string Process::Ram() {
  if (LinuxParser::Exists(pid)) ram = LinuxParser::Ram(pid);
  return ram;
}

// DONE: Return the user (name) that generated this process
string Process::User() {
  if (LinuxParser::Exists(pid)) user = LinuxParser::User(pid);
  return user;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
  if (LinuxParser::Exists(pid)) uptime = LinuxParser::UpTime(pid);
  return uptime;
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return a.cpuUsage < cpuUsage;
}

bool Process::operator==(const Process& a) const { return pid == a.pid; }

Process::Process(int p_pid) : pid(p_pid) {}
