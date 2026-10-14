// Author: Allison McDowell
// Date: 10/24/26
// Class: Operating Systems
// Assignment: Assignment 1 System Calls and User Space in xv6
// File: sleep.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]){
    //incorrect amount of arguments
    if(argc != 2){
        fprintf(2, "usage: sleep <ticks>\n");
        exit(1);
    }
    
    char *p = argv[1];
    
    //Argument is missing/null
    if(*p == '\0'){
       fprintf(2, "usage: sleep <ticks>\n");
       exit(1);
    }
     
    //Ticks is not fully digits
    while(*p != '\0'){
        if(*p <'0' || *p> '9'){
            fprintf(2, "usage: sleep <ticks>\n");
            exit(1);
        }
        p++;
    }

    int ticks = atoi(argv[1]);
    //The amount of ticks is negative
    if(pause(ticks) < 0) {
        fprintf(2, "usage: sleep <ticks>\n");
        exit(1);
    }


    exit(0);

}

