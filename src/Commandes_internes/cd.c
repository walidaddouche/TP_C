#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/cd.h"


void cd(const char *path, int *status, char *previous_dir) {
    const char *new_dir = NULL;
   
    //commande "cd" seule sans argument
    if (path == NULL) {
       //sauvegarde du répertoire actuel dans 'previous_dir' avant de changer de répertoire
       if (getcwd(previous_dir, CWDSIZE) == NULL) {
        perror("Erreur lors de la récupération du répertoire actuel");
        *status = 1;
        return;
       }
       //utilisation de la variable d'environnement HOME comme répertoire cible
       new_dir = getenv("HOME");
       if (!new_dir) {
            perror("variable $HOME introuvable\n");
            *status = 1;
            return;
        }
    } else if (strcmp(path, "-") == 0) {                                                                            
        if (previous_dir[0] == '\0') {//si 'previous_dir' est vide, cela signifie que on n'a pas exécuté une commande cd avant
            perror("pas de répertoire précédent (OLDPWD non défini)\n");
            *status = 1;
            return;
        }
        new_dir = previous_dir;//le répertoire cible est le répertoire où nous étions avant d'exécuter la commande cd avec/sans argument
    } else {//sinon, un chemin spécifique est fourni
         //sauvegarde du répertoire actuel dans 'previous_dir' avant de changer de répertoire
        if (getcwd(previous_dir, CWDSIZE) == NULL) {
        perror("Erreur lors de la récupération du répertoire actuel");
        *status = 1;
        return;
    }
        new_dir = path;
    }

   
    if (chdir(new_dir) != 0) {
        perror("Erreur lors du changement de répertoire");
        *status = 1;
        return;
    }
    //si tout se passe bien, le statut est mis à 0 pour indiquer un succès
    *status = 0;
}
