#ifndef MEMORY_H
#define MEMORY_H
#include "process.h"
#include <utility>
#include <vector>

class Memory{
public:
  Memory(const long long int &memSize);                          // initializes memoryLeft to memSize of OS
  void addProcess(const long long int &procSize, Process &p);    // add holes and decrement memoryLeft
  void removeProcess(const long long int &memStart, const long long int &memEnd, const long long int &procSize);   // remove from holes and increment memoryLeft
  long long int getMemoryLeft();                                 // to check if process is too big for memory remaining
  void addToHoles(std::pair<long long int,long long int> newP);  // have pairs of holes be added appropriately
  void sortHoles();
private:
  std::vector < std::pair<long long int, long long int> > holes;  // contains coordinates of holes
  long long int memoryLeft;                                       // total mem left in OS
  void mergePairs();                                              // merges overlapping pairs
  long long int bestFit(long long int procSize);                  // scans the holes list for the best fit for the procSize
                                                                  // then returns the starting mem address of that hole
};

#endif
