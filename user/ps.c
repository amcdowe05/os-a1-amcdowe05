//Allison McDowell
// 10/14/26
// Operating Systems
// System Calls
// ps.c

#include "kernel/types.h"
#include "kernel/uproc.h"
#include "user/user.h"

#define MAXPROC 64

char * state_name(int state){ 
    switch(state){
        case 1: return "used";
        case 2: return "sleeping";
        case 3: return "runnable";
        case 4; return "running";
        case 5; return "zombie";
        case 6; return "unknown";
    }
}

int main(void){
    struct uproc procs[MAXPROC];

    int n = getprocs(procs, MAXPROC);
    if(n < 0){
        fprintf(2, "ps: getprocs failed\n");
        exit(1);
    }

    printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");

    for(int i=0; i<n; i++){
        printf("%d\t%d\t%s\t%lu\t%\n", procs[i].pid, procs[i].ppid, state_name(procs[i].state), procs[i].sz, procs[i].name);
    }

    exit(0);

}
