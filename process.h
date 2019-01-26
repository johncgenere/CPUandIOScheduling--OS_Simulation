#ifndef PROCESS_H
#define PROCESS_H
#include <string>

class Process{
public:
  Process(const int &id, const int &level, const long long int &size);
  int getPID();
  long long int getMemSize();
  long long int getFirstMemAddress();
  long long int getLastMemAddress();
  int getPriorityLevel();
  std::string getFile();
  void setFile(const std::string &fileName);
  void setFirstMemAddress(const long long int &first);
  void setLastMemAddress(const long long int &last);
private:
  int PID;
  long long int memSize;
  long long int firstMemAddress;
  long long int lastMemAddress;
  int priorityLevel;
  std::string file;
};

#endif
