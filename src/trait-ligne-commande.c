#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h> 
#include "../includes/prompt.h"
#include "../includes/status.h"
#include "../includes/exit.h"
#include "../includes/pwd.h"
#include "../includes/ftype.h"
#include "../includes/cd.h"
#include"../includes/commande-for.h"
#include "../includes/cmd-externes.h"

#define MAX_ARGS 64

void execute_for(const char *ligne, int *status);

void parse_command(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        end[1] = '\0';
        if (*token != '\0') {
            argv[i++] = token;
        }

        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

void remove_spaces(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

void trait_ligne_commande(char *ligne, int *status, char *previous_dir) {
    ligne[strcspn(ligne, "\n")] = '\0'; 
    
    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    }
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;
        execution_exit(status, args);
    }
    else if (strncmp(ligne, "pwd", 3) == 0) {
        if (strncmp(ligne, "\0", 4) != 0) {
            fprintf(stderr, "pwd: ne prend pas des arguements\n");
        }
        else {
            execution_pwd();
        }
    }
    else if (strncmp(ligne, "ftype ", 6) == 0) {
        char *ref = ligne + 6;
        remove_spaces(ref);
        if (*ref == '\0') {
            fprintf(stderr, "ftype: invalid reference (empty after removing spaces)\n");
        } else {
            ftype(ref, status);
        }
    }
    else if (strncmp(ligne, "cd", 2) == 0) {
        char *path = ligne + 2;
        remove_spaces(path);
        if (*path == '\0') {
            path = NULL;
        }
        cd(path, status, previous_dir);
    }
    else if (strncmp(ligne, "for ", 4) == 0) {
        execute_for(ligne, status);
    }
    else { 
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            cmd_externes(argv, status);
        }
    }
}
