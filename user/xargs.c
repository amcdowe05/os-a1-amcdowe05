// Author: Allison McDowell
// Date: 10/14/26
// Class: Operating Systems
// Assignment: Assignment 1 System Calls and User Space in xv6
// File: xargs.c


#include "kernel/types.h"
#include "user/user.h"

#define MAXARG 32
#define BUFSIZE 4096

static char storage[BUFSIZE];
static char *words[MAXARG];

int run_command(char *baseArgs[], int baseCount, char *words[], int wordCount){

    char *args[MAXARG + 1];
    int i;

    if(wordCount == 0){
        return 0;
    }


    if(baseCount + wordCount > MAXARG){
        fprintf(2, "xargs: too many arguments\n");
        return 1;
    }
    for(i=0; i < baseCount; i++){
        args[i] = baseArgs[i];
    }

    for(int i = 0; i < wordCount; i++){
        args[baseCount + i] = words[i];
    }

    args[baseCount + wordCount] = 0;

    int pid = fork();

    if (pid < 0){
        fprintf(2, "xargs: fork failed\n");
        return 1;
    }

    if(pid == 0){
        exec(args[0], args);
        fprintf(2, "xargs: exec failed\n");
        exit(1);
    }

    int status;
    int waited = wait(&status);

    if(waited < 0 || status != 0){
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[]){

    int cmdIndex = 1;
    int batchSize = 0;
    int useBatchSize = 0;
    int failed = 0;

    if(argc >= 2 && strcmp(argv[1], "-n") == 0){
        if(argc < 4){
            fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
            exit(1);
        }

        char *p = argv[2];


        if(*p == '\0'){
            fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
            exit(1);
        }

        while(*p != '\0'){
            if(*p < '0' || *p > '9'){
                fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
                exit(1);
            }
            p++;
        }

        batchSize = atoi(argv[2]);

        if(batchSize < 1){
            fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
            exit(1);
        }

        useBatchSize = 1;
        cmdIndex = 3;
    }

    if(cmdIndex >= argc){
        fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
        exit(1);
    }
    char **baseArgs = &argv[cmdIndex];
    int baseCount = argc - cmdIndex;
    int maxWords = MAXARG - baseCount;

    if(maxWords < 1){
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    if(useBatchSize && batchSize > maxWords){
        batchSize = maxWords;
    }

 
    int used = 0;
    int wordCount = 0;
    int inWord = 0;
    int c;

    while(read(0, &c, 1) == 1){
        char ch = c;

        if(ch == ' ' || ch == '\t' || ch == '\n'){
            if(inWord){
                if(used >= BUFSIZE){
                    fprintf(2, "xargs: input too long\n");
                    exit(1);
                }
                storage[used++] = '\0';
                wordCount++;
                inWord = 0;
            }

            if((ch == '\n' && !useBatchSize && wordCount > 0) || (useBatchSize && wordCount == batchSize)){
                if(run_command(baseArgs, baseCount, words, wordCount) != 0){
                    failed = 1;
                }
                wordCount = 0;
                used = 0;

            }
            continue;

        }
        
        if(!inWord){
            if(wordCount >= maxWords){
                fprintf(2, "xargs: too many arguments\n");
                exit(1);
            }
            if(used >= BUFSIZE){
                fprintf(2, "xargs: input too long\n");
                exit(1);
            }

            words[wordCount] = &storage[used];
            inWord = 1;
        }
        if(used >= BUFSIZE -1){
            fprintf(2, "xargs: input too long\n");
            exit(1);
        }

        storage[used++] = ch;
    }
    if(inWord){
        storage[used++] = '\0';
        wordCount++;
    }

    if(wordCount > 0){
        if(run_command(baseArgs, baseCount, words, wordCount != 0)){
            failed = 1;
        }
    }

    exit(failed ? 1 : 0);
}
