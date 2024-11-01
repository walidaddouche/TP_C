// main.c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../includes/prompt.h"

void prompt(int status);

int main() {
    rl_outstream = stderr;
    int status = 0;
    pid_t pid;

    while (1) {
        prompt(status);
        char *ligne = readline("");

        if (!ligne) {
            printf("\n");
            break;
        }

        if (*ligne) {
            add_history(ligne);
            pid = fork();
            if (pid == 0) {
                char *commande[] = {ligne, NULL};
                execvp(commande[0], commande);
                perror("échoue commande"); // En cas d'échec d'execvp
                exit(1);
            } else if (pid > 0) {
                int wstatus;
                waitpid(pid, &wstatus, 0);
                
                if (WIFSIGNALED(wstatus)) {
                    status = 128 + WTERMSIG(wstatus); // Cas d'interruption par signal
                } else {
                    status = WEXITSTATUS(wstatus); // Retourne le code de sortiz
                }
            } else {
                perror("Erreur lors de la création du processus");
                status = 1;
            }
        }
        free(ligne);
    }
    return 0;
}
