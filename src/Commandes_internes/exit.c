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
        // Sinon, on essaie de convertir l'argument en entier et quitter avec ce code
        int exit_status = atoi(args);
        printf("Exit with status: %d\n", exit_status);
        exit(exit_status);  // Quitter avec le code d'erreur spécifié
    }
}

