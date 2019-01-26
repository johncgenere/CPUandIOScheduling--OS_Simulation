#include "memory.h"
#include <iostream>

Memory::Memory(const long long int &memSize):memoryLeft(memSize){
  std::pair <long long int, long long int> initial(0, memSize-1);
  holes.push_back(initial);            // initial hole is the entire memory
}

void Memory::addProcess(const long long int &procSize, Process &p){
  long long int first = bestFit(procSize);        // first will contain the first mem location in
  long long int second = (first + procSize) - 1;  // -1 so that it takes the correct space
  p.setFirstMemAddress(first);          // give process info
  p.setLastMemAddress(second);          // give process info
  std::pair <long long int, long long int> newP(first, second);
  addToHoles(newP);                 // add pair to used list and to hole list
  memoryLeft -= procSize;               // decrement memoryLeft;
}

void Memory::removeProcess(const long long int &memStart, const long long int &memEnd, const long long int &procSize){
  memoryLeft += procSize;                   // increment memoryLeft
  std::pair <long long int, long long int> newHole(memStart, memEnd);
  holes.insert(holes.begin(), newHole);
  sortHoles();
  mergePairs();                             // after removal of process, check for overlapping pairs
}

long long int Memory::getMemoryLeft(){return memoryLeft;}

long long int Memory::bestFit(long long int procSize){
  long long int best = memoryLeft;                  // total memory left in OS should be initial best
  long long int bestStart = 0;                          // will store firt mem address of best fit hole

  for(auto i:holes){                            // checks every hole
    long long int holeSize = (i.second - i.first) + 1;    // gets hole size
    if(holeSize >= procSize){                    // process size is too big
      if(holeSize <= best){                     // if current best is less than previous best, it will be the new best
        best = holeSize;                        // holeSize is now best fit
        bestStart = i.first;                    // gets the starting mem address of the best fit
      }
    }
  }
  return bestStart;
}

void Memory::addToHoles(std::pair<long long int, long long int> newP){
  for(int i = 0; i < holes.size(); i++){                      // checks every hole
    if(holes[i].second > newP.first){                         // stops when a hole has a end value greater than the first value of the pair passed in
      std::pair <long long int, long long int> hole((newP.second+1), holes[i].second); // create a new pair containing the second values of each pair
      holes[i].second = newP.first-1;                           // set current second of pair to first of passed in pair
      holes.insert(holes.begin()+(i+1), hole);                // insert new hole into set of holes
      break;                                                  // done searching
    }
  }

  // removes incorrect holes each time one is added
  for(int i = 0; i < holes.size(); i++){
    if(holes[i].first >= holes[i].second){
      holes.erase(holes.begin()+i);
    }
  }
}

void Memory::sortHoles(){
  std::pair <long long int, long long int> left;
  std::pair <long long int, long long int> current;

  for(int i = 0; i < holes.size(); i++){
    int oldI = i;
    for(int j = i - 1; j >= 0; j--){
      current = holes[i];
      left = holes[j];
      if(holes[i].first > holes[j].first)
        continue;
      if(holes[i].first < holes[j].first)
        holes[i] = left;
        holes[j] = current;
    }
    i = oldI;
  }
  mergePairs();
}

void Memory::mergePairs(){
  if(holes.size() < 2){                                     // only one hole, so no need to do anything to it
    return;
  }

  for(int i = 1; i < holes.size(); i++){                    // checks all holes starting from the second hole if there is one
    int previous = i - 1;                                   // this will refer to the previous hole
    if(holes[previous].second + 1 == holes[i].first){       // if they differ by one
      holes[previous].second = holes[i].second;             // merge
      holes.erase(holes.begin()+i);                         // remove additional pair
    }
  }
}
