#ifndef FOR_LOOP_H
#define FOR_LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_hidden(const char *filename);  // Vérifie si un fichier est caché
int has_extension(const char *filename, const char *ext);  // Vérifie si un fichier a l'extension spécifiée
int check_file_type(const char *path, char type);  // Vérifie le type de fichier
void list_files_recursive(const char *dir, int include_hidden, const char *ext, char type, char **argv, int *status);  // Liste les fichiers récursivement
void execute_for_loop(char *ligne, int *status);  // Exécute la boucle "for" avec les options spécifiées

#endif  // FOR_LOOP_H
