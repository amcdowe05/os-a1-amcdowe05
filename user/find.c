// Author: Allison McDowell
// Date: 10/14/26
// Class: Operating Systems
// Assignment: Assignment 1 System Calls and User Space in xv6
// File: find.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

#define MAXPATH 512

int match(char *pattern, char *name){

    if(*pattern == '\0'){
        return *name == '\0';
    }
    if(*pattern == '*'){
        return match(pattern + 1, name) || (*name != '\0' && match(pattern, name + 1));
    }
    if(*pattern == *name && *name != '\0'){
        return match(pattern + 1, name + 1);
    }
    return 0;

}

void find(char *path, char *pattern, int typeFilter, int isRoot){
    char buf[MAXPATH];
    char name[DIRSIZ + 1];
    struct dirent de;
    struct stat st;

    int fd = open(path, 0);
    if(fd < 0){
        if(isRoot){
            fprintf(2, "find: cannot open %s\n", path);
        }
        return;
    }
    if (fstat(fd, &st) < 0){
        close(fd);
        return;
    }
    if(st.type != T_DIR){
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }

        memmove(name, de.name, DIRSIZ);
        name[DIRSIZ] = '\0';

        for(int i=0; i<DIRSIZ; i++){
            if(name[i] == '\0'){
                break;
            }
        }

        if(strcmp(name, ".")== 0 || strcmp(name, "..") == 0){
            continue;
        }

        if(strlen(path) + 1 + strlen(name) >= sizeof(buf)){
            continue;
        }
        strcpy(buf, path);
        int len = strlen(buf);
        buf[len] = '/';
        buf[len + 1] = '\0';
        memmove(buf + len + 1, name, strlen(name) + 1);

        if(stat(buf, &st) < 0){
            printf("DEBUG path: %s\n", buf);
            continue;
        }

        if(match(pattern, name) && (typeFilter == 0 || (typeFilter == 'f' && (st.type == T_FILE || st.type == T_DEVICE)) || (typeFilter == 'd' && st.type == T_DIR))) {
            printf("%s\n", buf);
        }
        if(st.type == T_DIR){
            find(buf, pattern, typeFilter, 0);
        }

    }
    close(fd);

}

int main(int argc, char *argv[]){

    int typeFilter = 0;
    if(argc != 3 && argc != 5){
        fprintf(2, "usage: find <dir> <pattern> [-type f|d]\n");
        exit(1);
    } 

    if(argc == 5){
        if(strcmp(argv[3], "-type") != 0 || (strcmp(argv[4], "f") != 0 && strcmp(argv[4], "d") != 0)) {
            fprintf(2, "usage: find <dir> <pattern> [-type f|d]\n");
            exit(1);
        }

        typeFilter = argv[4][0];
    }

    int fd = open(argv[1], 0);

    if(fd < 0){
        fprintf(2, "find: cannot open %s\n", argv[1]);
        exit(1);
    }
    close(fd);

    find(argv[1], argv[2], typeFilter, 1);
    exit(0);
}

