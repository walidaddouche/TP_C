#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "../includes/prompt.h"
#include "../includes/status.h"
#include "../includes/exit.h"
#include "../includes/pwd.h"
#include "../includes/ftype.h"
#include "../includes/cd.h"

#define MAX_ARGS 64

void parse_command(char *line, char **argv);
void execute_external(char **argv, int *status);
void analyse_and_execute(char *cmd, int *status,char *previous_dir);
void prompt(int status);

int main() {
    rl_outstream = stderr;
    char *ligne;
    int status = 0; 
    static char previous_dir[CWDSIZE] = "";


    while (1) {
        prompt(status);  
        ligne = readline("");  

        if (!ligne) {
            printf("\n");
            execution_exit(&status, NULL); 
            break;
        }
        if (*ligne) {
            add_history(ligne); 
            analyse_and_execute(ligne,&status,previous_dir); 
        }
        free(ligne);
    }

    return 0;
}


void analyse_and_execute(char *ligne, int *status, char *previous_dir) {
    ligne[strcspn(ligne, "\n")] = '\0'; 

    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status,NULL); 
    }
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;  
        execution_exit(status, args); 
    }
    else if (strcmp(ligne, "pwd") == 0) {
        execution_pwd(); 
    }
    else if (strncmp(ligne, "ftype", 5) == 0) {
        char *ref = ligne + 6; 
        ftype(ref);
    }

   else if (strncmp(ligne, "cd", 2) == 0) {
        char current_dir[CWDSIZE];
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("Erreur lors de la récupération du répertoire actuel");
            return 1; 
        }

        char *lastSlash = strrchr(current_dir, '/');
            if (lastSlash != NULL) {
                *lastSlash = '\0'; 
            }
            strncpy(previous_dir, current_dir, CWDSIZE); 



        char *path = ligne + 2;
        while (*path == ' ') path++;
        if (*path == '\0') {
            path = NULL;
        }
        cd(path, status, previous_dir);
    }

    else {
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            execute_external(argv, status);
        }
    }
}
void parse_command(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

void execute_external(char **argv, int *status) {
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
