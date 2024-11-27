#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void ftype(const char *ref,int *status) {
    struct stat file_info;
    if (lstat(ref, &file_info) == -1) {
        perror("ftype");
        *status = 1;  
        return;
    }
    if (S_ISDIR(file_info.st_mode)) {
        printf("directory\n");
        *status=0;
    }
    else if (S_ISREG(file_info.st_mode)) {
        printf("regular file\n");
        *status=0;
    }
    else if (S_ISLNK(file_info.st_mode)) {
        printf("symbolic link\n");
        *status=0;
    }
    else if (S_ISFIFO(file_info.st_mode)) {
        printf("named pipe\n");
        *status=0;
    }
    else {
        printf("other\n");
        *status=0;

    }}
