#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../includes/cd.h"  // Inclure le header pour la fonction cd

#define CWDSIZE 1024

// Fonction pour changer de répertoire
void execution_cd(const char *path, int *status) {
    char current_dir[CWDSIZE];
    static char previous_dir[CWDSIZE] = "";  // Utilisation d'une variable statique pour garder une trace du précédent répertoire

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("Erreur lors de la récupération du répertoire actuel");
        *status = 1;  // Erreur
        return;
    }

    // Si path est NULL, cela signifie que l'on doit aller au répertoire HOME
    const char *new_dir = path;

    if (path == NULL) {
        new_dir = getenv("HOME");  // Aller au répertoire HOME
        if (!new_dir) {
            fprintf(stderr, "Erreur: variable $HOME introuvable\n");
            *status = 1;  // Erreur
            return;
        }
    } else if (strcmp(path, "-") == 0) {
        // Si le chemin est "-", on doit revenir au répertoire précédent
        if (previous_dir[0] == '\0') {
            fprintf(stderr, "Erreur: pas de répertoire précédent\n");
            *status = 1;  // Erreur
            return;
        }
        new_dir = previous_dir;
    }

    // Essayer de changer de répertoire
    if (chdir(new_dir) != 0) {
        perror("Erreur lors du changement de répertoire");
        *status = 1;  // Erreur
        return;
    }

    // Sauvegarder le répertoire courant comme répertoire précédent
    strncpy(previous_dir, current_dir, CWDSIZE);
    *status = 0;  // Succès
}
