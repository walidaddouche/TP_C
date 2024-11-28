#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../../includes/status.h"

void execution_exit(int *status, char *args) {
    if (args == NULL) {
        exit(*status);
    } else {
        char *cv_arg;
        long exit_status = strtol(args, &cv_arg, 10);

        if (*cv_arg != '\0') {
            fprintf(stderr, "exit: argument invalide: %s\n", args);
            exit(1);
        }

        if (exit_status < 0 || exit_status > 255) {
            fprintf(stderr, "exit: code de sortie hors plage (0-255): %ld\n", exit_status);
            exit(1);
        }
        exit((int)exit_status);
    }
}
