#include <string>

#include "format.h"
#include <iomanip>

using std::string;


// Display OUTPUT in HH:MM:SS Formate.
string Format::ElapsedTime(long seconds) { 
     long Seconds, Hours, Minutes;

Seconds = seconds % 60;
Minutes = seconds / 60;
Hours = Minutes / 60;
Minutes = Minutes % 60;

std::ostringstream stream;
  stream  << std::setfill('0') << std::setw(2) << Hours << ":" 
     << std::setfill('0') << std::setw(2) << Minutes << ":"
     << std::setfill('0') << std::setw(2) << Seconds;

  return stream.str();
 
    }