#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

// Basic class for Process representation

class Process {
 public:
// constructor
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  
 private:

int pid_;
std::string user_;
std::string command_;
std::string ram_;
float cpuUtilization_;
long int upTime_;

};

#endif