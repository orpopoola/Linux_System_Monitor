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

template <typename Z> //Template to check stol is not operating on an invalid argument
Z convertStr(string x) {
  Z value;
  if (x.length()==0){x="0";}
  try {
    value = stol(x);
  } catch (const std::invalid_argument) {
    value = 0;
  }
  return value;
}

Process::Process(int pid) : pid_(pid) {}
// Returns this process's ID
int Process::Pid() { return pid_; }

// Returns this process's CPU utilization
float Process::CpuUtilization() {
  long totalTime = LinuxParser::ActiveJiffies(pid_);
  long seconds = LinuxParser::UpTime(pid_);
  return float(totalTime / sysconf(_SC_CLK_TCK)) / seconds;
}

// Returns the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Returns this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Returns the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Returns the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

//  Overloada th "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return convertStr<long>(LinuxParser::Ram(pid_)) > convertStr<long>(LinuxParser::Ram(a.pid_));
}