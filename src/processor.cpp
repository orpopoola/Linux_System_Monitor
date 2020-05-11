#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {
    double utilization{0};
    long active_ticks = LinuxParser::ActiveJiffies();
    long idle_ticks = LinuxParser::IdleJiffies();
    long duration_active{active_ticks - cached_active_ticks_};
    long duration_idle{idle_ticks - cached_idle_ticks_};
    long duration{duration_active + duration_idle};
    (duration == 0)? utilization =0:utilization = static_cast<float>(duration_active)/duration;
    cached_active_ticks_ = active_ticks;
    cached_idle_ticks_ = idle_ticks;
    return utilization; 
 }