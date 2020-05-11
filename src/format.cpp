#include <string>
#include <math.h>
#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int days_rem, hours, mins, secs;
    string s_hours, s_mins, s_secs;
    days_rem = seconds%86400;
    hours =floor(days_rem/3600);
    mins = floor((days_rem-3600*hours)/60);
    secs = days_rem - 3600*hours-60*mins;
    (hours<10)?s_hours="0"+to_string(hours):s_hours=to_string(hours);
    (mins<10)?s_mins = "0"+to_string(mins):s_mins=to_string(mins);
    (secs<10)?s_secs = "0"+to_string(secs):s_secs=to_string(secs);
    return s_hours+":"+s_mins+":"+s_secs;
 }