#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/cd.h"


void cd(const char *path, int *status, char *previous_dir) {
    const char *new_dir = NULL;
   

    if (path == NULL) {
        
       if (getcwd(previous_dir, CWDSIZE) == NULL) {
        perror("Erreur lors de la récupération du répertoire actuel");
        *status = 1;
        return;
       }
       new_dir = getenv("HOME");
       if (!new_dir) {
            perror("variable $HOME introuvable\n");
            *status = 1;
            return;
        }
    } else if (strcmp(path, "-") == 0) {                                                                            
        if (previous_dir[0] == '\0') {
            perror("pas de répertoire précédent (OLDPWD non défini)\n");
            *status = 1;
            return;
        }
        new_dir = previous_dir;
    } else {
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

    *status = 0;
}
