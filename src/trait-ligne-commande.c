#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/prompt.h"
#include "../includes/status.h"
#include "../includes/exit.h"
#include "../includes/pwd.h"
#include "../includes/ftype.h"
#include "../includes/cd.h"
#include "../includes/commande-for.h"
#include "../includes/cmd-externes.h"
#include "../includes/commande_consecutive.h"

#define MAX_ARGS 64
#define MAX_PATH_LEN 1024

// Fonction pour découper une commande en arguments
void parse_command(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

// Fonction pour traiter les lignes de commande
void trait_ligne_commande(char *ligne, int *status, char *previous_dir) {
    // Supprimer le retour à la ligne à la fin de la commande
    ligne[strcspn(ligne, "\n")] = '\0';

    // Si la commande est structurée, on la découpe et traite chaque sous-commande
    if (strchr(ligne, ';') != NULL) {
        char *commands[MAX_ARGS];
        int num_cmds = 0;

        execute_consecutive_cmd(ligne, commands, &num_cmds);

        for (int i = 0; i < num_cmds; i++) {
            char *trimmed_cmd = commands[i];
            while (*trimmed_cmd == ' ') trimmed_cmd++; // Supprimer les espaces au début
            if (*trimmed_cmd != '\0') { // Ignorer les commandes vides
                trait_ligne_commande(trimmed_cmd, status, previous_dir);
            }
            free(commands[i]); // Libérer la mémoire
        }

        // À la fin, le `status` reflète la dernière commande exécutée
        return;
    }

    // Commande interne : exit
    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    } else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;
        execution_exit(status, args);
    }
    // Commande interne : pwd
    else if (strncmp(ligne, "pwd", 3) == 0) {
        char *ptr = ligne + 3;
        while (*ptr == ' ') ptr++;
        if (*ptr != '\0') {
            printf("Erreur dans la syntaxe de pwd\n");
            *status = 1;
            return;
        }
        execution_pwd();
        *status = 0; // Supposons que pwd réussit toujours
    }
    // Commande interne : ftype
    else if (strncmp(ligne, "ftype", 5) == 0) {
        char ref[MAX_PATH_LEN], *ptr;
        int i = 0;
        ptr = ligne + 5;
        while (*ptr == ' ') ptr++;
        if (*ptr == '\0') {
            printf("Erreur dans la syntaxe de ftype : argument manquant\n");
            *status = 1;
            return;
        }
        while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
            ref[i++] = *ptr++;
        }
        ref[i] = '\0';
        while (*ptr == ' ') ptr++;
        if (*ptr != '\0') {
            printf("Erreur dans la syntaxe de ftype : argument malformé\n");
            *status = 1;
            return;
        }
        ftype(ref, status);
    }
    // Commande interne : cd
    else if (strncmp(ligne, "cd", 2) == 0) {
        char ref[MAX_PATH_LEN], *ptr;
        int i = 0;
        ptr = ligne + 2;
        while (*ptr == ' ') ptr++;
        if (*ptr == '\0') {
            cd(NULL, status, previous_dir);
        } else {
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
    // Commande interne : boucle for
    else if (strncmp(ligne, "for", 3) == 0) {
        execute_for(ligne, status);
    }
    // Commande externe
    else {
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            cmd_externes(argv, status);
        }
    }
}

