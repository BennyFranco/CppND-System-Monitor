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
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator==(const Process& a) const;

  // TODO: Declare any necessary private members
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