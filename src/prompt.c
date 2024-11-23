#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define VERT "\001\033[32m\002"
#define RED "\001\033[91m\002"
#define BLEU "\001\033[36m\002"
#define NORMAL "\001\033[00m\002"
#define CWDSIZE 1024

void prompt(int status) {
    char *path = malloc(CWDSIZE);
    if (!path) {
        perror("malloc");
        return;
    }

    char *color_status = (status == 0) ? VERT : RED;
    char display_status[50]; 

    if (status == 0) {
        snprintf(display_status, sizeof(display_status), "0");
    } else if (status >= 128) {
        snprintf(display_status, sizeof(display_status), "SIG(%s)", strsignal(status - 128));
    } else {
        snprintf(display_status, sizeof(display_status), "%d", status);
    }

    if (getcwd(path, CWDSIZE) == NULL) {
        perror("getcwd");
        free(path);
        return;
    }

    fprintf(stderr, "%s[%s]%s", color_status, display_status, BLEU);

    if (strlen(path) > 22) {
        fprintf(stderr, "...%s", path + strlen(path) - 22);
    } else {
        fprintf(stderr, "%s", path);
    }

    fprintf(stderr, NORMAL "$ ");
    fflush(stderr);

    free(path);
}
