#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/cd.h"

// Fonction pour changer de répertoire
void cd(const char *path, int *status, char *previous_dir)  {
    char current_dir[CWDSIZE];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("Erreur lors de la récupération du répertoire actuel");
        *status = 1;
        return;
    }

    // Si path est NULL, cela signifie que l'on doit aller au répertoire HOME
    const char *new_dir = path;

    if (path == NULL) {
        new_dir = getenv("HOME");
        if (!new_dir) {
            fprintf(stderr, "Erreur: variable $HOME introuvable\n");
            *status = 1;
            return;
        }
    } else if (strcmp(path, "-") == 0) {
        if (previous_dir[0] == '\0') {
            fprintf(stderr, "Erreur: pas de répertoire précédent\n");
            *status = 1;
            return;
        }
        new_dir = previous_dir;
    }

    if (chdir(new_dir) != 0) {
        perror("Erreur lors du changement de répertoire");
        *status = 1;
        return;
    }

    // Sauvegarder le répertoire courant comme répertoire précédent
    strncpy(previous_dir, current_dir, CWDSIZE);
    *status = 0;
}
