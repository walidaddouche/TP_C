#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void ftype(const char *ref) {
    struct stat file_info;

    // Vérifier si le fichier existe
    if (lstat(ref, &file_info) == -1) {
        perror("ftype");
        return;
    }

    // Déterminer le type de fichier
    if (S_ISDIR(file_info.st_mode)) {
        printf("directory\n");
    }
    else if (S_ISREG(file_info.st_mode)) {
        printf("regular file\n");
    }
    else if (S_ISLNK(file_info.st_mode)) {
        printf("symbolic link\n");
    }
    else if (S_ISFIFO(file_info.st_mode)) {
        printf("named pipe\n");
    }
    else if (S_ISCHR(file_info.st_mode)) {
        printf("character device\n");
    }
    else if (S_ISBLK(file_info.st_mode)) {
        printf("block device\n");
    }
    else {
        printf("other\n");
    }
}
