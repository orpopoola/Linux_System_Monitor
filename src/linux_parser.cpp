#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
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
  string os, tag, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> tag >>kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {   
    string line;
    string key;
    string value;
    float mem_tot, mem_free;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "MemTotal:") {
            mem_tot = stof(value);
          }
          if (key == "MemFree:"){
            mem_free = stof(value);
          }
        }
      }
      return (mem_tot-mem_free)/mem_tot;
    }
    return 0.;
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {  
  string up_time, amt_time; 
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time>>amt_time;
    return stol(up_time);
  }
  return stol(up_time);
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total = 0;
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  for(int i = kUser_; i <= kSteal_; i++) {//adding from 0 (kUser_) to 8(kSteal_)
    total += stol(cpu_util[i]);
  }
  return total;  }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {  
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for(int i = kIdle_; i <= kIOwait_; i++) { //For idle, add up third(kIdle_) and fourth(kIOwait_) cols of 
    total += stol(cpuUtilization[i]);
  }
  return total;  }

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
  return cpu_util; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 04;} //code for non-accessible file

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0.;
  }//code for non-accessible file

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, value);
    return value;
  }return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {   
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory +to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") { //most processes have VmData=0, so I did not use it because that provides little information
          return to_string(stoi(value)/1024);
        }
      }
    }
  }
  return "N/A";}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {   
  string key, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory +to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  } return value;}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {  
  string uid = LinuxParser::Uid(pid);
  string key,x, value, line;
  vector<string> cpu_util;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> x >>key) {
        if (key == uid) {
          return value;
        }
      }
    }
  } return value;} //prevent return void

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string value, line;
  std::ifstream filestream(kProcDirectory +to_string(pid)+ kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> value;
      }
  }
  return (LinuxParser::UpTime()-(std::stol(value)/sysconf(_SC_CLK_TCK)));}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string value, line;
  long total=0;
  std::ifstream filestream(kProcDirectory +to_string(pid)+ kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> value;
      if (i > 12 && i < 17) {
        total += stol(value);
      }
      }
      return total;
  }
  return (LinuxParser::UpTime()-(std::stol(value)/sysconf(_SC_CLK_TCK))); }