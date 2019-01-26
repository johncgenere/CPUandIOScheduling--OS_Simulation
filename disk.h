#ifndef DISK_H
#define DISK_H
#include <queue>

class Disk{
public:
  Disk();                                 // initializes hard drive to be 0;
  void setCurrentProcess(const int &PID); // sets new process running on HD
  int getCurrentProcess();                // to check current process running on HD
  void addToIOQ(const int &PID);          // during d command, and drive is not idle, add process to queue
  void popIOQ();                          // when drive becomes idle, pop from I/O queue; during D command
  std::queue<int> getIOQ();               // for memory info
  bool isQueueEmpty();                    // used when HDD is idle
private:
  int currentProcess;    // process ID
  std::queue <int> ioQ;  // FCFS
};

#endif
