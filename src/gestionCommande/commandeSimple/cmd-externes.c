#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "../../../includes/prompt.h"
#include "../../../includes/status.h"
#include "../../../includes/exit.h"
#include "../../../includes/pwd.h"
#include "../../../includes/ftype.h"
#include "../../../includes/cd.h"

#define MAX_ARGS 64

void cmd_externes(char **argv, int *status) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
        perror("Échec de la commande");
        exit(1);
    } else if (pid > 0) {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        if (WIFSIGNALED(wstatus)) {
            *status = 128 + WTERMSIG(wstatus);
        } else {
            *status = WEXITSTATUS(wstatus);
        }
    } else {
        perror("Erreur lors du fork");
        *status = 1;
    }
}
