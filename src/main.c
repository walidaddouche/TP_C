#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "../includes/prompt.h"
#include "../includes/status.h"
#include "../includes/exit.h"
#include "../includes/pwd.h"
#include "../includes/ftype.h"
#include "../includes/cd.h"

void analyse_and_execute(char *ligne, int *status, char *previous_dir);
void prompt(int status);

int main() {
    rl_outstream = stderr;
    char *ligne;
    int status = 0;  // Initialisation du statut à 0 au début (c'est la valeur de retour affichée dans le prompt)
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
            analyse_and_execute(ligne, &status, previous_dir);  // Analyser et exécuter la commande
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
        execution_exit(status, NULL);  // Exécution de la commande exit
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
            return;
        }

        char *lastSlash = strrchr(current_dir, '/');
        if (lastSlash != NULL) {
            *lastSlash = '\0';  // Découper le chemin pour obtenir le répertoire parent
        }
        strncpy(previous_dir, current_dir, CWDSIZE);  // Sauvegarder le répertoire parent dans previous_dir

        char *path = ligne + 2;
        while (*path == ' ') path++;  // Ignore leading spaces
        if (*path == '\0') {
            path = NULL;  // Si aucun argument, on va au répertoire HOME
        }
        cd(path, status, previous_dir);  // Exécuter la commande cd
    }
    else if (strncmp(ligne, "for ", 4) == 0) {
        // Gestion de la commande "for"
        char *cmd_args = ligne + 4;  // Exclure "for " de la commande
        char *commande = strtok(cmd_args, " ");  // Extraire la commande principale (par exemple, "cd")
        char *arg2 = strtok(NULL, " ");  // Deuxième argument possible (chemin ou nombre d'itérations)
        char *iterations_str = strtok(NULL, " ");  // Troisième argument (nombre d'itérations)

        if (commande == NULL) {
            fprintf(stderr, "Erreur: format attendu 'for [commande] [argument] [nombre d'itérations]'\n");
            return;
        }

        int iterations = 0;  // Initialisation du nombre d'itérations
        char *path = NULL;   // Chemin pour la commande "cd"

        // Déterminer le rôle des arguments
        if (arg2 != NULL && isdigit(arg2[0])) {
            // Si le second argument est un nombre, le chemin est vide
            iterations = atoi(arg2);
        } else {
            // Si le second argument n'est pas un nombre, il est considéré comme un chemin
            path = arg2;
            if (iterations_str != NULL && isdigit(iterations_str[0])) {
                iterations = atoi(iterations_str);
            } else {
                fprintf(stderr, "Erreur: nombre d'itérations invalide ou manquant.\n");
                return;
            }
        }

        if (iterations <= 0) {
            fprintf(stderr, "Erreur: nombre d'itérations invalide (%d).\n", iterations);
            return;
        }
        printf("Exécution de la boucle for %d fois pour la commande '%s %s'.\n", iterations, commande, path ? path : "");
        
        char current_dir[CWDSIZE];
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
           perror("Erreur lors de la récupération du répertoire actuel");
           return;
        }
       
        for (int i = 0; i < iterations; i++) {
            printf("Itération %d: Exécution de la commande '%s %s'\n", i + 1, commande, path ? path : "");
            
            if (strcmp(commande, "cd") == 0) {
              
                char *lastSlash = strrchr(current_dir, '/');
                if (lastSlash != NULL) {
                *lastSlash = '\0'; // Découper le chemin pour obtenir le répertoire parent
                }
                strncpy(previous_dir, current_dir, CWDSIZE);  // Sauvegarder le répertoire parent
                cd(path, status, previous_dir);  // Exécuter la commande cd
                if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
                printf("Répertoire actuel après cd: %s\n", current_dir);  // Affichage du répertoire courant
            } else {
                perror("Erreur lors de la récupération du répertoire actuel");
            }
             }    
            else if (strcmp(commande, "pwd") == 0) {
                execution_pwd();
            }   
            else if (strcmp(commande, "ftype") == 0) {
                if (path != NULL) {
                    ftype(path);
                }   
                else {
                   fprintf(stderr, "Erreur: 'ftype' nécessite un fichier en argument.\n");
                }
            } 
            else if (strcmp(commande, "exit") == 0) {
                 execution_exit(status, path);  // Appel de la fonction exit avec l'argument donné
            } 
            else {
                 fprintf(stderr, "Erreur: Commande '%s' non supportée dans la boucle for.\n", commande);
            }
    }
}
    // Si ce n'est pas une commande interne, lancer une commande externe
    else {
        pid_t pid = fork();
        if (pid == 0) {  // Processus enfant
            char *args[] = {ligne, NULL};  // Argument de la commande
            execvp(args[0], args);  // Exécution de la commande externe
            perror("Échec de la commande");  // Si execvp échoue
            exit(1);
        } else if (pid > 0) {  // Processus parent
            int wstatus;
            waitpid(pid, &wstatus, 0);  // Attendre que l'enfant se termine
            if (WIFSIGNALED(wstatus)) {
                *status = 128 + WTERMSIG(wstatus);  // Si le processus a été tué par un signal
            } else {
                *status = WEXITSTATUS(wstatus);  // Code de sortie du processus enfant
            }
        } else {
            perror("Erreur lors du fork");
            *status = 1;
        }
    }
}
