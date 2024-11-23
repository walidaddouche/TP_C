#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/cd.h"

void cd(const char *path, int *status, char *previous_dir) {
    char current_dir[CWDSIZE];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        fprintf(stderr, "cd: erreur lors de la récupération du répertoire actuel: %s\n", strerror(errno));
        *status = 1;
        return;
    }

    const char *new_dir = path;

    if (path == NULL) {
        new_dir = getenv("HOME");
        if (!new_dir) {
            fprintf(stderr, "cd: HOME not set\n");
            *status = 1;
            return;
        }
    } else if (strcmp(path, "-") == 0) {
        if (previous_dir[0] == '\0') {
            fprintf(stderr, "cd: OLDPWD not set\n");
            *status = 1;
            return;
        }
        new_dir = previous_dir;
    }

    if (chdir(new_dir) != 0) {
        fprintf(stderr, "cd: %s\n", strerror(errno));
        *status = 1;
        return;
    }

    strncpy(previous_dir, current_dir, CWDSIZE);
    *status = 0;
}
