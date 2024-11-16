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
    int status = 0;  // Initialisation du statut à 0 au début (c'est la valeir de retour afficher dans le prompt)
    static char previous_dir[CWDSIZE] = "";


    while (1) {
        prompt(status);  // Afficher le prompt avec le statut
        ligne = readline("");   // Lire la ligne de commande

        if (!ligne) {
            printf("\n");
            execution_exit(&status, NULL);  // Appel à exit avec le statut actuel
            break;
        }

        if (*ligne) {
            add_history(ligne);  // Ajouter à l'historique des commandes
            analyse_and_execute(ligne,&status,previous_dir);  // Analyser et exécuter la commande
        }
        free(ligne);
    }

    return 0;
}

// Fonction d'analyse et d'exécution des commandes
void analyse_and_execute(char *ligne, int *status, char *previous_dir) {
    ligne[strcspn(ligne, "\n")] = '\0';  // Supprimer le saut de ligne

    // Gestion des commandes internes
    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status,NULL);  // Exécution de la commande exit
    }
    else if (strncmp(ligne, "exit ", 5) == 0) {
        // Si l'utilisateur tape "exit [VAL]", on passe le [VAL] en argument
        char *args = ligne + 5;  // Exclure "exit " de la commande
        execution_exit(status, args);  // Exécution de la commande exit avec argument
    }
    else if (strcmp(ligne, "pwd") == 0) {
        execution_pwd();  // Exécution de la commande pwd
    }
    else if (strncmp(ligne, "ftype", 5) == 0) {
        char *ref = ligne + 6;  // Récupérer la référence après "ftype "
        ftype(ref);  // Appeler la fonction ftype pour afficher le type du fichier
    }

    else if (strncmp(ligne, "cd", 2) == 0) {
        char current_dir[CWDSIZE];
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("Erreur lors de la récupération du répertoire actuel");
            return 1; 
        }

        char *lastSlash = strrchr(current_dir, '/');
            if (lastSlash != NULL) {
                *lastSlash = '\0'; // Découper le chemin pour obtenir le répertoire parent
            }
            strncpy(previous_dir, current_dir, CWDSIZE); // Sauvegarder le répertoire parent dans previous_dir



        char *path = ligne + 2;
        while (*path == ' ') path++;  // Ignore leading spaces
        if (*path == '\0') {
            path = NULL;  // Si aucun argument, on va au répertoire HOME
        }
        cd(path, status, previous_dir); // Exécuter la commande cd
    }
    // Si ce n'est pas une commande interne, lancer une commande externe
    else {
        // Gestion des commandes externes
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);
        if (argv[0] != NULL) {
            execute_external(argv, status);
        }
    }
}

// Fonction pour découper une ligne en tokens
void parse_command(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;  // Terminez le tableau par NULL pour execvp
}

// Gérer les commandes externes
void execute_external(char **argv, int *status) {
    pid_t pid = fork();
    if (pid == 0) {  
        execvp(argv[0], argv);  // Exécution de la commande avec ses arguments
        perror("Échec de la commande");  // Si execvp échoue
        exit(1);
    } else if (pid > 0) {  
        int wstatus;
        waitpid(pid, &wstatus, 0);  // Attendre que l'enfant se termine
        if (WIFSIGNALED(wstatus)) {
            *status = 128 + WTERMSIG(wstatus);  // Si le processus a été tué par un signal
        } else {
            *status = WEXITSTATUS(wstatus);  
        }
    } else {
        perror("Erreur lors du fork");
        *status = 1;
    }
}

