#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//  Read and return the Operatingsystem
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

//  Read and return the Kernel
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  std::string line;
  std::string key;
  float value;
  std::string kb;
  float MemTotal;
  float MemFree;

  std::ifstream filestream (kProcDirectory+kMeminfoFilename);
if(filestream.is_open()){
  while(getline(filestream,line)){ 
    std::istringstream linestream (line);
    linestream >> key >> value >>kb;
    if(key== "MemTotal:"){
      MemTotal= value;
    }
    if(key== "MemFree:"){
      MemFree= value;
    }
  }
}
  return (MemTotal-MemFree)/MemTotal; 
  }
    

//Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  long t1,t2;

  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if(filestream.is_open()){
filestream >> t1 >> t2 ;
return t1;
  }
  return t1; 
  }

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
  }


//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
   vector<long> value = LinuxParser::CpuUtilization();
  
  return value[0]+value[1]+value[2]+value[5]+value[6]+value[7];  
   }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
   vector<long> value = LinuxParser::CpuUtilization();
  
  return value[3]+value[4];  
   }

//Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 

  std::string line;
  long v;
  std::string key;
  std::vector<long> value;
  
 
std::ifstream FileStream(kProcDirectory+kStatFilename);
    
    if(FileStream.is_open()){
        
        while(std::getline(FileStream,line)){
            std::istringstream LineStream(line);
            LineStream >> key;
            if(key=="cpu"){
            while(LineStream >> v){

               value.push_back(v);
            }
          }
        } 
    }
    
  return value; 
  }

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string line;
  std::string key;
  int value;

  std::ifstream filestream (kProcDirectory+kStatFilename);
if(filestream.is_open()){
  while(getline(filestream,line)){
    
    std::istringstream linestream (line);
    linestream >> key >> value;
    if(key== "processes"){
      return value;
    }
  }
}
  return value; 
  }

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string line;
  std::string key;
  int value;

  std::ifstream filestream (kProcDirectory+kStatFilename);
if(filestream.is_open()){
  while(getline(filestream,line)){
    
    std::istringstream linestream (line);
    linestream >> key >> value;
    if(key== "procs_running"){
      return value;
    }
  }
}
  return value; 
  }

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  
  std::string value;

  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(filestream.is_open()){
filestream >> value ;
return value;
  }
  return value; 
  }

//Read and return the memory used by a process
// using VmRSS instead of VmSize.
string LinuxParser::Ram(int pid) { 

  std::string line;
  std::string key;
  int value;
    
std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatusFilename);
    
    if(FileStream.is_open()){
        
        while(std::getline(FileStream,line)){
            std::istringstream LineStream(line);
            LineStream >> key >> value;
            if(key=="VmRSS:"){

               return std::to_string((int)(value*0.001));
            }
        } 
    }
    return std::to_string((int)(value*0.001));
  
  }

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string line;
  std::string key;
  std::string value;
    
std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatusFilename);
    
    if(FileStream.is_open()){
        
        while(std::getline(FileStream,line)){
            std::istringstream LineStream(line);
            LineStream >> key >> value;
            if(key=="Uid:"){

               return value;
            }
        } 
    }
    return value;
  }

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 

std::string line;
std::string key1;
std::string key2;
std::string value;
    
    std::ifstream FileStream(kPasswordPath);
    if(FileStream.is_open()){
        while(getline(FileStream,line)){
            std::replace(line.begin(), line.end() , ':', ' ');
            std::istringstream LineStream(line);
            LineStream >> value >> key1 >> key2;
            if(key2==LinuxParser::Uid(pid)){
                return value;
            }
        }
    }
    return value;
  }

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 

  std::string line;
  long v1ong;
  std::string vstring;
 
  std::vector<long> value;
  
 
std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
    
    if(FileStream.is_open()){
        
        while(std::getline(FileStream,line)){
            std::istringstream LineStream(line);
            LineStream >> v1ong >> vstring >> vstring;
            while(LineStream >> v1ong){

               value.push_back(v1ong);
            }
        } 
    }
    
  long starttime = value[18];
 
  return LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK)); 
  }

float LinuxParser::CpuUtilization(int pid){

  std::string line;
  long vint;
  std::string vstring;
  std::vector<int> value;
  
 
std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
    
    if(FileStream.is_open()){
        
        while(std::getline(FileStream,line)){
            std::istringstream LineStream(line);
            LineStream >> vint >> vstring >> vstring;
            while(LineStream >> vint){

               value.push_back(vint);
            }
        } 
    }
    
  int utime = value[10];
  int stime = value[11];
  int cutime = value[12];
  int cstime = value[13];
  int starttime = value[18];
    
  float total_time = utime + stime + cutime + cstime;
  float seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
   
return (total_time/sysconf(_SC_CLK_TCK))/seconds;

}