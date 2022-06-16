#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memory = 0.0f;
  string line;
  std::map<string, float> meminfo;
  std::ifstream file{kProcDirectory + kMeminfoFilename};

  if (file.is_open()) {
    // I only read the first two lines:
    // - MemTotal
    // - MemFree
    for (size_t i = 0; i < 2; i++) {
      std::getline(file, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      string key;
      int value;
      lineStream >> key >> value;
      meminfo.insert(std::pair<string, float>(key, value));
    }

    float memoryUsed = meminfo["MemTotal"] - meminfo["MemFree"];
    memory = (memoryUsed / meminfo["MemTotal"]);
  }
  return memory;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptimeInSeconds{};
  std::ifstream file{kProcDirectory + kUptimeFilename};
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream lineStream(line);
    lineStream >> uptimeInSeconds;
  }
  return uptimeInSeconds;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long utime = GetProcStatValue(LinuxParser::kUTime, pid);
  long stime = GetProcStatValue(LinuxParser::kSTime, pid);
  long cutime = GetProcStatValue(LinuxParser::kCUTime, pid);
  long cstime = GetProcStatValue(LinuxParser::kCSTime, pid);

  return utime + stime + cutime + cstime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jeffies = CpuUtilization();
  long nonIdle = jeffies[CPUStates::kUser_] + jeffies[CPUStates::kNice_] +
                 jeffies[CPUStates::kSystem_] + jeffies[CPUStates::kIRQ_] +
                 jeffies[CPUStates::kSoftIRQ_] + jeffies[CPUStates::kSteal_];

  return nonIdle;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jeffies = CpuUtilization();
  long idle = jeffies[CPUStates::kIdle_] + jeffies[CPUStates::kIOwait_];

  return idle;
}

// DONE: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  string line;
  std::ifstream file{kProcDirectory + kStatFilename};
  vector<long> jeffies;

  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream lineStream(line);
    string buffer;
    while (lineStream >> buffer) {
      if (buffer == "cpu") continue;
      jeffies.push_back(std::stol(buffer));
    }
  }
  return jeffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key{"processes"};
  std::ifstream file{kProcDirectory + kStatFilename};

  int value = GetFileValue<int>(key, file);
  return value;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key{"procs_running"};
  std::ifstream file{kProcDirectory + kStatFilename};

  int value = GetFileValue<int>(key, file);
  return value;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd, line;
  std::ifstream file{kProcDirectory + to_string(pid) + kCmdlineFilename};
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream lineStream(line);
    lineStream >> cmd;
  }
  return cmd;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key{"VmSize"};
  std::ifstream file{kProcDirectory + to_string(pid) + kStatusFilename};
  auto vmSize = GetFileValue<float>(key, file);
  vmSize /= 1024.f;
  auto result = Format::FmtString<float>("%.2f", vmSize);
  return result;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key{"Uid"};
  std::ifstream file{kProcDirectory + to_string(pid) + kStatusFilename};

  return GetFileValue<std::string>(key, file);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, username;
  std::ifstream passwd{kPasswordPath};
  if (passwd.is_open()) {
    while (std::getline(passwd, line)) {
      if (line.find(":" + Uid(pid) + ":") != string::npos) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream lineStream(line);
        lineStream >> username;
      }
    }
  }

  return username;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long utime = GetProcStatValue(LinuxParser::kUTime, pid);
  long stime = GetProcStatValue(LinuxParser::kSTime, pid);

  return (utime + stime) / sysconf(_SC_CLK_TCK);
}

template <typename T>
T LinuxParser::GetFileValue(const std::string& key, std::ifstream& file) {
  string line, foundKey;
  T value = {};
  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line.find(key) != string::npos) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream lineStream(line);
        lineStream >> foundKey >> value;
      }
    }
  }
  return value;
}
long LinuxParser::GetProcStatValue(LinuxParser::ProcessStat processStat,
                                   int pid) {
  string line;
  long value{0};
  std::ifstream procStatStream{kProcDirectory + to_string(pid) + kStatFilename};

  if (procStatStream.is_open()) {
    std::getline(procStatStream, line);
    std::istringstream lineStream(line);

    int index = 0;
    for (string buffer; lineStream >> buffer; index++) {
      if (index == processStat) {
        value = std::stol(buffer);
        break;
      }
    }
  }
  return value;
}
int LinuxParser::CpuCores() {
  string key{"siblings"};
  std::ifstream file{kProcDirectory + kCpuinfoFilename};
  return std::stoi(GetFileValue<std::string>(key, file));
}
bool LinuxParser::Exists(int pid) {
  auto buffer = LinuxParser::Pids();
  return std::find(buffer.begin(), buffer.end(), pid) != buffer.end();
}
