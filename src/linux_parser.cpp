#include "linux_parser.h"

#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename P>
P getValueOfFile(std::string const &filename) {
  std::string line;
  P value;
  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

template <typename Z>
Z convertStr(string x) {
  Z value;
  try {
    value = stol(x);
  } catch (const std::invalid_argument) {
    value = 0;
  }
  return value;
}

template <typename Z>
Z convertStri(string x) {
  Z value;
  try {
    value = stoi(x);
  } catch (const std::invalid_argument) {
    value = 0;
  }
  return value;
}

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
  string os, tag, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> tag >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = convertStr<int>(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total =
      findValueByKey<float>(memTotal, kMeminfoFilename);  // "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
}

long LinuxParser::UpTime() {
  string up_time, amt_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> amt_time;
    return convertStr<long>(up_time);
  }
  return convertStr<long>(up_time);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long total = 0;
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <= kSteal_;
       i++) {  // adding from 0 (kUser_) to 8(kSteal_)
    total += convertStr<long>(cpu_util[i]);
  }
  return total;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int i = kIdle_; i <= kIOwait_;
       i++) {  // For idle, add up third(kIdle_) and fourth(kIOwait_) cols of
    total += convertStr<long>(cpuUtilization[i]);
  }
  return total;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string value, line;
  vector<string> cpu_util;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        cpu_util.push_back(value);
      }
    }
  }
  return cpu_util;
}

int LinuxParser::TotalProcesses() {
  string pro_css = "processes";
  int total = findValueByKey<int>(pro_css, kStatFilename);
  return total;
}

int LinuxParser::RunningProcesses() {
  string pro_run = "procs_running";
  int running = findValueByKey<int>(pro_run, kStatFilename);
  return running;
}

string LinuxParser::Command(int pid) {
  string cmd_str;
  cmd_str = std::string(
      getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
  return (cmd_str.length()>50)? cmd_str.substr(0,50)+"...":cmd_str; //truncate excessively (>50)long string
}

string LinuxParser::Ram(int pid) {
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key ==
            "VmSize:") {  // most processes have VmData=0, so I did not use it
                          // because that provides little information
          return to_string(convertStr<int>(value) / 1024);
        }
      }
    }
  }
  return "N/A";
}

string LinuxParser::Uid(int pid) {
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string key, x, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> x >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}  // prevent return void

long LinuxParser::UpTime(int pid) {
  string value, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> value;
    }
  }
  return (LinuxParser::UpTime() -
          (convertStr<long>(value) / sysconf(_SC_CLK_TCK)));
}

long LinuxParser::ActiveJiffies(int pid) {
  string value, line;
  long total = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> value;
      if (i > 12 && i < 17) {
        total += convertStr<long>(value);
      }
    }
    return total;
  }
  return (LinuxParser::UpTime() -
          (convertStr<long>(value) / sysconf(_SC_CLK_TCK)));
}