#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
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
#define MAX_PATH_LEN 1024
void execute_for(const char *ligne, int *status);
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

    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    }
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;
        execution_exit(status, args);
    }
    else if (strncmp(ligne, "pwd",3) == 0) {
        char *ptr;
        ptr = ligne + 3;
        while (*ptr == ' ') ptr++;
        if (*ptr != '\0') {
           printf("erreur dans la syntaxe de pwd\n");
           *status=1;
           return;
        }
        execution_pwd();
  }
    else if (strncmp(ligne, "ftype", 5) == 0) {
       char ref[MAX_PATH_LEN], *ptr;
       int i = 0;
       ptr = ligne + 5;
       while (*ptr == ' ') ptr++;
       if (*ptr == '\0') {
          printf("erreur dans la syntaxe de ftype : argument manquant\n");
          *status = 1;
          return;
       }
       while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
          ref[i++] = *ptr++;
       }
       ref[i] = '\0';
       while (*ptr == ' ') ptr++;
       if (*ptr != '\0') {
          printf("erreur dans la syntaxe de ftype : argument malformé\n");
          *status = 1;
          return;
       }
       if (i == 0) {
          printf("erreur dans la syntaxe de ftype : référence vide\n");
          *status = 1;
          return;
       }
 ftype(ref, status);
   }

    else if (strncmp(ligne, "cd", 2) == 0) {
        char ref[MAX_PATH_LEN], *ptr;
        int i = 0;
        ptr = ligne + 2;
        while (*ptr == ' ') ptr++;
        if (*ptr == '\0') {cd(NULL, status, previous_dir);}
        else{
           while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
             ref[i++] = *ptr++;
           }
           ref[i] = '\0';
           while (*ptr == ' ') ptr++;
           if (*ptr != '\0') {
             printf("Erreur dans la syntaxe de cd : arguments malformés\n");
             *status = 1;
             return;
           }
           cd(ref, status, previous_dir);
       }
    }
     else if (strncmp(ligne, "for", 3) == 0) {
        execute_for(ligne,status);
    }
 else {
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            cmd_externes(argv, status);
        }
    }
}
