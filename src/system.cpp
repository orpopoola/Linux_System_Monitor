#include <unistd.h>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::map;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    /*vector<int> pids(LinuxParser::Pids());

    //Create a set
    set<int> extant_pids;
    for (Process const& process:processes_){
        extant_pids.insert(process.Pids());
    }

    //Emplace all new processes
    for(int pid:pids){
        if(extant_pids.find(pid) == extant_pids.end())
            processes_.emplace_back(pid);
    }

    //Update CPU Utilization
    for(auto& process:processes_){
        process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pids()) - LinuxParser::Jiffies());
    }
    std::sort(processes_.begin(), processes_.end(), std::greater=10); */
 
    auto pids_list = LinuxParser::Pids();
    processes_.clear();
    for (auto pid : pids_list){
        processes_.emplace_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end(),[](Process& p1, Process& p2){return (p1<p2) ;});
    return processes_;  }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();
    }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }