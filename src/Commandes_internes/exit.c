#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../../includes/status.h"

//fonction pour exécuter la commande "exit" avec un code de statut donné
void execution_exit(int *status, char *args) {
    //si aucun argument n'est passé à la commande exit, on sort avec le statut actuel
    if (args == NULL) {
        exit(*status);  //utilise le statut actuel pour quitter
    } else {
        //si un argument est passé, on essaie de le convertir en (long)
        char *cv_arg;  //pointeur pour vérifier si tout l'argument a été converti
        long exit_status = strtol(args, &cv_arg, 10);  // Convertir l'argument en nombre (base 10)

        //si l'argument ne correspond pas à un nombre entier valide, on affiche une erreur
        if (*cv_arg != '\0') {
            fprintf(stderr, "exit: argument invalide: %s\n", args);
            exit(1);  //on quitte avec un code d'erreur (1)
        }

        //si le code de sortie est en dehors de la plage autorisée (0-255), on affiche une erreur
        if (exit_status < 0 || exit_status > 255) {
            fprintf(stderr, "exit: code de sortie hors plage (0-255): %ld\n", exit_status);
            exit(1);  //on quitte avec un code d'erreur (1)
        }

        //si tout est valide, on quitte avec le code de statut spécifié
        exit((int)exit_status);  //convertir exit_status en entier et quitter avec ce code
    }
}
