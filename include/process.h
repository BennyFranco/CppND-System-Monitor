#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int p_pid);
  int Pid() const;                         // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  bool operator==(const Process& a) const;

  // DONE: Declare any necessary private members
 private:
  int pid;
  long activeJiffiesCache{0};
  long systemJiffiesCache{0};

  std::string user;
  std::string command;
  float cpuUsage{0};
  std::string ram;
  long int uptime{0};
};

#endif