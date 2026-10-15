// Author: Allison McDowell
// Date: 10/14/26
// Class: Operating Systems
// Assignment: Assignment 1 System Calls
// File: trace.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(2, "usage: trace mask command\n";
        exit(1);
    }

    int mask = atoi(argv[1]);
    trace(mask);
    exec(argv[2], &argv[2]);
    fprintf(2, "exec failed\n");
    exit(1);
}
