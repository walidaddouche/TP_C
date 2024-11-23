#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../../includes/status.h"

// Fonction pour gérer la commande `exit`
void execution_exit(int *status, char *args) {
    if (args == NULL) {
        // Si aucun argument n'est fourni, quitter avec le statut courant
        exit(*status);
    } else {
        // Convertir l'argument en entier avec validation
        char *cv_arg;
        long exit_status = strtol(args, &cv_arg, 10);

        // Vérifier si la conversion est valide
        if (*cv_arg != '\0') {
            fprintf(stderr, "exit: argument invalide: %s\n", args);
            exit(1);
        }

        // Vérifier que le code de sortie est dans la plage valide [0, 255]
        if (exit_status < 0 || exit_status > 255) {
            fprintf(stderr, "exit: code de sortie hors plage (0-255): %ld\n", exit_status);
            exit(1);
        }

        // Quitter avec le code de sortie spécifié
        exit((int)exit_status);
    }
}
