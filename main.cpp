#include "process.h"
#include "disk.h"
#include "memory.h"
#include <iostream>     // cout, cin
#include <queue>        // priority_queue
#include <vector>       // vector
#include <cctype>       // isdigit()
#include <string>       // stoll(), stoi(), getline()

void sim(const long long int &sizeOfOS, const int &amountOfHDD);
void initializeHDD(std::vector <Disk> &HDD, const int &amountOfHDD);
bool preemptive(int &CPU, const std::vector<Process> &allPCBs, Process &p, std::priority_queue<int> &readyQ);
bool cpuIdle(const int &CPU);
void updateIdleCPU(int &CPU, const std::vector <Process> &allPCBs, std::priority_queue<int> &readyQ);
void A(const std::string &command, int &CPU, int &pid, std::vector <Process> &allPCBs, Memory &m, std::priority_queue<int> &readyQ);
void d(const std::string &command, int &CPU, std::vector<Process> &allPCBs, std::vector<Disk> &HDD);
void terminate(int &CPU, std::vector<Process> &allPCBs, Memory &m);
void IORequest(int &CPU, const int &HDNum, std::vector<Disk> &HDD);
void IODone(const std::string &command, std::vector<Disk> &HDD, const std::vector<Process> &allPCBs, int &CPU, std::priority_queue<int> &readyQ);
void CPUStatus(const int &CPU, const std::vector<Process> &allPCBs, const std::priority_queue<int> &readyQ);
void IOStatus(const std::vector <Process> &allPCBs, std::vector <Disk> &HDD);
void MemoryStatus(const std::vector<Process> &allPCBs);

int main(){
  long long int sizeOfOS = 0;
  int amountOfHDD = 0;

  std::cout << "How much memory is this OS going to have?\n";
  std::cin >> sizeOfOS;
  std::cout << "How many hard drives is this OS going to have\n";
  std::cin >> amountOfHDD;

  sim(sizeOfOS, amountOfHDD);
}

void sim(const long long int &sizeOfOS, const int &amountOfHDD){
  int CPU = 0;                        // represents process ID using CPU
  static int pid = 1;                 // iterates throughout this function
  std::string command;                // for commands given by the user
  Memory m(sizeOfOS);                 // initialize memory of OS
  std::priority_queue <int> readyQ;   // Ready Queue
  std::vector <Process> allPCBs;      // Contains all existing Processes
  std::vector <Disk> HDD;             // Contains all hard disk

  initializeHDD(HDD, amountOfHDD);    // HDD now has the right amount of elements

  while(std::getline(std::cin,command)){
    static int pid = 1;               // for process ID numbers, incremented in A
    switch (command[0]) {
      case 'A':
        A(command, CPU, pid, allPCBs, m, readyQ);
        break;
      case 't':
        terminate(CPU, allPCBs, m);   // removes process, check memory
        break;
      case 'd':
        d(command, CPU, allPCBs, HDD);
        break;
      case 'D':
        IODone(command, HDD, allPCBs, CPU, readyQ);
        break;
      case 'S':
      {
        switch (command[2]) {
          case 'r':
            CPUStatus(CPU, allPCBs, readyQ);
            break;
          case 'i':
            IOStatus(allPCBs, HDD);
            break;
          case 'm':
            MemoryStatus(allPCBs);
            break;
          default:
            std::cout << "ERROR--No such command\n";
        }
        break;
      }
    }
    updateIdleCPU(CPU, allPCBs, readyQ);
  }
}

void initializeHDD(std::vector <Disk> &HDD, const int &amountOfHDD){
  for(int i = 0; i < amountOfHDD; i++){
    Disk d;
    HDD.push_back(d);
  }
}

bool preemptive(int &CPU, const std::vector<Process> &allPCBs, Process &p, std::priority_queue<int> &readyQ){
  if(CPU == 0)
    return false;                                   // for first process of OS

  for(auto i:allPCBs){
    if(i.getPID() == CPU){                          // finds current process on CPU
      if(i.getPriorityLevel() < p.getPriorityLevel()){   // checks if current process has a higher priority level than the new process
        CPU = p.getPID();                           // if it is, give CPU to new process
        readyQ.push(i.getPriorityLevel());          // add previous process to ready queue by priority
        return true;                                // return true so that new process is not added to readyQ
      }
      return false;                                 // return false so that new process is added to readyQ
    }
  }
}

bool cpuIdle(const int &CPU){
  if(CPU == 0)
    return true;
  else
    return false;
}

void updateIdleCPU(int &CPU, const std::vector <Process> &allPCBs, std::priority_queue <int> &readyQ){
  if(cpuIdle(CPU) && !readyQ.empty()){
    for(auto i:allPCBs){      // all process's
      if(i.getPriorityLevel() == readyQ.top()){
        CPU = i.getPID();     // CPU gets PID of next priority process
        readyQ.pop();         // process priority gets popped
        break;                // stop checking
      }
    }
  }
}

void A(const std::string &command, int &CPU, int &pid, std::vector <Process> &allPCBs, Memory &m, std::priority_queue <int> &readyQ){
  std::string priority, size;
  long long int nSize = 0;
  int nPriority = 0;

  if(isdigit(command[1]) || isalpha(command[1])){
    std::cout << "Invalid Command\n";
    return;                         // try another command
  }

  for(int i = 2; i < command.length(); i++){
    if(isdigit(command[i]))
      priority += command[i];
    else{
      for(int j = i; j < command.length(); j++){
        if(isdigit(command[j]))
          size += command[j];
      }
      break;
    }
  }

  nSize = std::stoll(size);
  nPriority = std::stoi(priority);

  if(nSize > m.getMemoryLeft()){
    std::cout << "ERROR--Process Size is more than Memory Left\n";
    return;
  }

  Process p(pid, nPriority, nSize);       // initialize new process
  m.addProcess(nSize, p);                 // add to memory and set memory addresses for process
  allPCBs.push_back(p);                   // push into vector
  pid++;

  if(!preemptive(CPU, allPCBs, p, readyQ)){           // if new priority is higher, preempt
    readyQ.push(nPriority);    // for each start of a process the priority queue adds the proirity of it
  }
}

void d(const std::string &command, int &CPU, std::vector<Process> &allPCBs, std::vector<Disk> &HDD){
  if(CPU == 0){
    std::cout << "ERROR--CPU is empty, no process to send to HDD\n";
    return;
  }

  std::string HDNumber;
  std::string fileName;
  int HDNum = HDD.size();
  if(isdigit(command[1]) || isalpha(command[1])){
    std::cout << "ERROR--No Such Command Exist\n";
    return;
  }

  for(int i = 2; i < command.length(); i++){
    if(isdigit(command[i]))
      HDNumber += command[i];
    else{
      for(int j = i; j < command.length(); j++){
        fileName += command[j];
      }
      break;
    }
  }

  if(!HDNumber.empty())
    HDNum = stoi(HDNumber);
  else{
    std::cout << "ERROR--HDD Number Can Not Be A Non-Number\n";
    return;
  }


  for(int i = 0; i < allPCBs.size(); i++){      // all processes
    if(CPU == allPCBs[i].getPID()){  // found current process on CPU
      allPCBs[i].setFile(fileName);  // gives it the file given
    }
  }

  IORequest(CPU, HDNum, HDD);  // will put current process on CPU in HD but if HD is full needs to go into queue
}

void terminate(int &CPU, std::vector<Process> &allPCBs, Memory &m){
  if(CPU == 0){
    std::cout << "ERROR--No Process On CPU to terminate\n";
  }
  for(int i = 0; i < allPCBs.size(); i++){  // iterates through all existing processes
    if(allPCBs[i].getPID() == CPU){         // finds current process running on CPU
      m.removeProcess(allPCBs[i].getFirstMemAddress(), allPCBs[i].getLastMemAddress(), allPCBs[i].getMemSize());  // remove from memory
      allPCBs.erase(allPCBs.begin()+i);     // removes current running CPU process
      CPU = 0;                              // Free CPU
      break;                                // Stop searching
    }
  }
}

void IORequest(int &CPU, const int &HDNum, std::vector<Disk> &HDD){
  if(HDNum >= HDD.size()){    // error handling
    std::cout << "ERROR--HDD Number Does Not Exist\n";
    return;
  }

  if(HDD[HDNum].getCurrentProcess() == 0){   // when currentProcess is at 0, it means that is idle
    HDD[HDNum].setCurrentProcess(CPU);       // if idle, set currentProcess to process pid requesting to use it
  }
  else{
    // add process to I/O queue when current HDD is not idle
    HDD[HDNum].addToIOQ(CPU);
  }

  CPU = 0;                   // make CPU available
}

void IODone(const std::string &command, std::vector<Disk> &HDD, const std::vector<Process> &allPCBs, int &CPU, std::priority_queue<int> &readyQ){
  std::string HDNumber;
  int HDNum = HDD.size();       // initialized to an invalid HDD number

  if(isdigit(command[1]) || isalpha(command[1])){
    std::cout << "ERROR--No Such Command Exist\n";
    return;
  }

  for(int j = 2; j < command.length(); j++){
    if(isdigit(command[j])){
      HDNumber += command[j];   // if not a digit, skipped
      HDNum = stoi(HDNumber);   // updates until end of string NOTE: Do this for the rest to avoid errors
    }
  }

  if(HDNum >= HDD.size()){
    std::cout << "ERROR--HDD Number Does Not Exist\n";
    return;
  }

  // before you do anything, get pid leaving HD and add it to the priority queue/ready-queue
  for(auto i:allPCBs){                                  // all processes
    if(i.getPID() == HDD[HDNum].getCurrentProcess()){   // found process on HDD
      HDD[HDNum].setCurrentProcess(0);                  // hard disk is now empty
      if(!preemptive(CPU, allPCBs, i, readyQ)){         // check if we need to preempt this process onto the CPU
        readyQ.push(i.getPriorityLevel());              // if not just send it to the readyQ
      }
      break;                              // stops searching
    }
  }

  if(!HDD[HDNum].isQueueEmpty())
    HDD[HDNum].popIOQ();           // sets new process in HDD and pops queue
}

void CPUStatus(const int &CPU, const std::vector<Process> &allPCBs, const std::priority_queue<int> &readyQ){
  if(CPU == 0 && readyQ.empty()){
    std::cout << "CPU is Currently Idle and Ready-Queue is Empty" << std::endl;
    return;
  }

  for(auto i:allPCBs){
    if(CPU == i.getPID()){
      std::cout << "Current Process Running on CPU: " << std::endl;
      std::cout << "PID: " << i.getPID() << " " << "Priority: " << i.getPriorityLevel() << std::endl;
      break;   // no need to check every other process
    }
  }

  if(!readyQ.empty()){
    std::priority_queue <int> temp = readyQ;
    std::cout << "Processes on the Ready-Queue: " << std::endl;

    while(!temp.empty()){
      int currentPriority = temp.top();

      for(auto i:allPCBs){
        if(i.getPriorityLevel() == currentPriority){
          std::cout << "PID: " << i.getPID() << " " << "Priority: " << i.getPriorityLevel() << std::endl;
          break;
        }
      }
      temp.pop();
    }
  }
}

void IOStatus(const std::vector <Process> &allPCBs, std::vector <Disk> &HDD){
  for(int i = 0; i < HDD.size(); i++){         // checks all HDD
    if(HDD[i].getCurrentProcess() == 0)        // idle hard disk won't print
      continue;
    else{                                      // hard drive is not idle
      for(auto j:allPCBs){                     // all processes
        if(HDD[i].getCurrentProcess() == j.getPID()){    // process in hard drive found in allPCBs
          std::cout << "Disk " << i << ": PID " << j.getPID() << " File: " << j.getFile() << std::endl;
          break;
        }
      }

      if(!HDD[i].isQueueEmpty()){                 // if I/O Queue is not empty
        std::queue<int> temp = HDD[i].getIOQ();   // ioQ is given to temp to not make changes
        std::cout << "Processes in I/O Queue: " << std::endl;

        while(!temp.empty()){                     // while is not empty
          for(auto x:allPCBs){                    // check all processes
            if(x.getPID() == temp.front()){       // process found in I/O Queue
              std::cout << "PID: " << x.getPID() << " File: " << x.getFile() << std::endl;
              temp.pop();                         // pop temp queue
            }
          }
        }
      }
    }
  }
}

void MemoryStatus(const std::vector<Process> &allPCBs){
  for(auto i:allPCBs){
    std::cout << "PID " << i.getPID() << ": " << i.getFirstMemAddress() << "-" << i.getLastMemAddress() << std::endl;
  }
}
