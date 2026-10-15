// Author: Allison McDowell
// Date: 10/14/26
// Class: Operating Systems
// Assignment: Assignment 1 System Calls
// File: sysinfo.h

struct sysinfo {
    uint64 freemem;
    uint64 nproc;
    uint64 nopenfile;
};
