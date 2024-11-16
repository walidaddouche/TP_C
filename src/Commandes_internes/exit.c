#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../../includes/status.h"

// Fonction pour gérer la commande `exit`
void execution_exit(int *status,char *args) {
       // Si l'argument est nul, on utilise la valeur de status comme code de sortie
    if (args == NULL) {
        printf("Exit with status: %d\n", *status);
        exit(*status);  // Quitter avec le code de statut
    } else {
        // Convertir l'argument en entier avec validation
        char *cv_arg;
        long exit_status = strtol(args, &cv_arg, 10);  // Conversion de l'argument en entier long

        // Vérifier si la conversion est valide
        if (*cv_arg != '\0') {
            fprintf(stderr, "Erreur: argument invalide pour exit: %s\n", args);
            exit(1);  // Quitter avec un code d'erreur par défaut
        }

        // Vérifier que le code de sortie est dans la plage valide [0, 255]
        if (exit_status < 0 || exit_status > 255) {
            fprintf(stderr, "Erreur: code de sortie hors plage (0-255): %ld\n", exit_status);
            exit(1);  // Quitter avec un code d'erreur par défaut
        }

        // Quitter avec le code de sortie spécifié
        printf("Exit with status: %ld\n", exit_status);
        exit((int)exit_status);
    }
}

