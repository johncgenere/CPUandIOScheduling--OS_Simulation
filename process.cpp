#include "process.h"

Process::Process(const int &id, const int &level, const long long int &size):PID(id),priorityLevel(level),memSize(size){}

int Process::getPID(){return PID;}

long long int Process::getMemSize(){return memSize;}

long long int Process::getFirstMemAddress(){return firstMemAddress;}

long long int Process::getLastMemAddress(){return lastMemAddress;}

int Process::getPriorityLevel(){return priorityLevel;}

std::string Process::getFile(){return file;}

void Process::setFile(const std::string &fileName){
  file = fileName;
}

void Process::setFirstMemAddress(const long long int &first){
  firstMemAddress = first;
}

void Process::setLastMemAddress(const long long int &last){
  lastMemAddress = last;
}
