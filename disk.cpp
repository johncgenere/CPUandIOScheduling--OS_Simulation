#include "disk.h"
#include <queue>

Disk::Disk():currentProcess(0){}

void Disk::setCurrentProcess(const int &PID){
  currentProcess = PID;
}

int Disk::getCurrentProcess(){return currentProcess;}

void Disk::addToIOQ(const int &PID){
  ioQ.push(PID);
}

void Disk::popIOQ(){
  currentProcess = ioQ.front();
  ioQ.pop();
}

std::queue<int> Disk::getIOQ(){
  return ioQ;
}

bool Disk::isQueueEmpty(){
  if(!ioQ.empty())
    return false;
  else
    return true;
}
