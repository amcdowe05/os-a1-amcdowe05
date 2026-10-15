// Allison McDowell
// 10/14/26
// Operating Systems
// System Calls
// uproc.h

struct uproc {
  int pid;
  int ppid;
  int state;
  uint64 sz;
  char name[16];
};
