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
#include "../includes/cmd-externes.h"
#include "../includes/cmd-for.h"
#define MAX_ARGS 64
#define CWDSIZE 1024

void cmd_externes(char **argv, int *status);

void parse_command(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

void trait_ligne_commande(char *ligne, int *status, char *previous_dir) {
    ligne[strcspn(ligne, "\n")] = '\0'; 
    if (strncmp(ligne, "for", 3) == 0) {
        char *path=ligne+3;
        cmd_for(path,status);
    }
   else if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    }
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;
        execution_exit(status, args);
    }
    else if (strcmp(ligne, "pwd") == 0) {
        execution_pwd();
    }
    else if (strncmp(ligne, "ftype ", 6) == 0) {
        char *ref = ligne + 6;
        ftype(ref);
    }
    else if (strncmp(ligne, "cd", 2) == 0) {
        // Gestion de `cd`
        char *path = ligne + 2;
        while (*path == ' ') path++; // Ignore les espaces
        if (*path == '\0') {
            path = NULL; // Si aucun chemin spécifié, HOME sera utilisé
        }
        cd(path, status, previous_dir);
    }
     else if (strncmp(ligne, "cd", 2) == 0) {
        
    else {
        // Gestion des commandes externes
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            cmd_externes(argv, status);
        }
    }
}
