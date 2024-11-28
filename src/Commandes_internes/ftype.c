#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void ftype(const char *ref,int *status) {
    struct stat file_info;
    if (lstat(ref, &file_info) == -1) {//verifier si l'appel a lstat echoue
        perror("ftype");
        *status = 1;  
        return;
    }
    if (S_ISDIR(file_info.st_mode)) {//verifier si le fichier est un repertoir 
        printf("directory\n");
        *status=0;
    }
    else if (S_ISREG(file_info.st_mode)) {//verifier si le fichier est un fichier non special
        printf("regular file\n");
        *status=0;
    }
    else if (S_ISLNK(file_info.st_mode)) {//verifier si le fichier est un lien symbolique 
        printf("symbolic link\n");
        *status=0;
    }
    else if (S_ISFIFO(file_info.st_mode)) {//verifier si le fichier est un fifo
        printf("named pipe\n");
        *status=0;
    }
    else { //si le fichier est d'un autre type
        printf("other\n");
        *status=0;

    }}
