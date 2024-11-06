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
void analyse_and_execute(char *cmd, int *status);
void prompt(int status);
int main() {
    rl_outstream = stderr;
    char *ligne;
    int status = 0;  // Initialisation du statut à 0 au début

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
            analyse_and_execute(ligne,&status);  // Analyser et exécuter la commande
        }
        free(ligne);
    }

    return 0;
}

// Fonction d'analyse et d'exécution des commandes
void analyse_and_execute(char *ligne, int *status) {
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
    // Si ce n'est pas une commande interne, lancer une commande externe
    else {
        pid_t pid = fork();
        if (pid == 0) {  // Processus enfant
            char *args[] = {ligne, NULL};  // Argument de la commande
            execvp(args[0], args);  // Exécution de la commande externe
            perror("échoue commande");  // Si execvp échoue
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

