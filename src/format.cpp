#include <string>
#include <math.h>
#include "format.h"
#include <stdexcept>

using std::string;
using std::to_string;

// Helper function that takes in input and produces a formatted output as shown below
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    try{
    int days_rem, hours, mins, secs;
    string s_hours, s_mins, s_secs;
    days_rem = seconds%86400; //Incase input is over a day (24hrs or 86400 seconds), we dump them here
    hours =floor(days_rem/3600);
    mins = floor((days_rem-3600*hours)/60);
    secs = days_rem - 3600*hours-60*mins;
    (hours<10)?s_hours="0"+to_string(hours):s_hours=to_string(hours);
    (mins<10)?s_mins = "0"+to_string(mins):s_mins=to_string(mins);
    (secs<10)?s_secs = "0"+to_string(secs):s_secs=to_string(secs);
    return s_hours+":"+s_mins+":"+s_secs;
    }
    catch(std::invalid_argument){return 0;}
 }