#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024
#define MAX_ARGS 256

// Vérifie si une chaîne contient une variable
int contient_variable(const char *template, const char *variable) {
    return strstr(template, variable) != NULL;
}

// Remplace toutes les occurrences d'une variable par une valeur donnée
void remplacer_toutes_occurrences(const char *template, const char *variable, const char *replacement, char *result, size_t result_size) {
    const char *current = template;
    char *dest = result;
    size_t remaining = result_size - 1;
    size_t var_len = strlen(variable);

    while (*current && remaining > 0) {
        if (strncmp(current, variable, var_len) == 0) {
            size_t repl_len = strlen(replacement);
            if (repl_len <= remaining) {
                strncpy(dest, replacement, repl_len);
                dest += repl_len;
                current += var_len;
                remaining -= repl_len;
            } else {
                break;
            }
        } else {
            *dest++ = *current++;
            remaining--;
        }
    }
    *dest = '\0';

    // Ajouter "do" et "done" pour fermer correctement la boucle for
    char tmp_result[MAX_ARGS];
    snprintf(tmp_result, sizeof(tmp_result), "for %s in %s; do %s done", variable, replacement, result);
    strncpy(result, tmp_result, result_size);
}

// Fonction pour parcourir un répertoire et exécuter une commande sur chaque élément
void traverser_repertoire(const char *dir_path, const char *cmd_template, const char *variable, int *status, int include_hidden, int recursive, char file_type_filter, const char *ext_filter) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire ");
        *status = 1;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer les fichiers cachés si non demandé
        if (entry->d_name[0] == '.' && !include_hidden) continue;

        // Construire le chemin complet
        char path[MAX_PATH_LEN];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (stat(path, &st) != 0) {
            perror("Erreur lors de l'accès à un fichier/répertoire");
            continue;
        }

        // Filtrer par type de fichier
        if ((file_type_filter == 'f' && !S_ISREG(st.st_mode)) ||
            (file_type_filter == 'd' && !S_ISDIR(st.st_mode)))
            continue;

        // Filtrer par extension (si spécifiée)
        if (ext_filter && !strstr(entry->d_name, ext_filter)) continue;

        // Construire la commande
        char command[MAX_ARGS];
        remplacer_toutes_occurrences(cmd_template, variable, path, command, sizeof(command));
        printf("Exécution de la commande : %s\n", command);
        if (system(command) != 0) {
            fprintf(stderr, "Erreur lors de l'exécution de la commande : %s\n", command);
            *status = 1;
        }

        // Parcours récursif si demandé et si l'élément est un répertoire
        if (recursive && S_ISDIR(st.st_mode)) {
            traverser_repertoire(path, cmd_template, variable, status, include_hidden, recursive, file_type_filter, ext_filter);
        }
    }
    closedir(dir);
}

// Fonction pour traiter et exécuter la commande "for ... { ... }"
void execute_for(const char *ligne, int *status) {
    const char *ptr = ligne + 3; // Sauter "for"

    // Ignorer les espaces au début
    while (*ptr == ' ') ptr++;

    // Lire la variable
    char variable[64];
    int i = 0;
    while (*ptr != ' ' && *ptr != '\0') {
        variable[i++] = *ptr++;
    }
    variable[i] = '\0';

    // Vérifier la syntaxe "in"
    while (*ptr == ' ') ptr++;
    if (strncmp(ptr, "in", 2) != 0 || *(ptr + 2) != ' ') {
        fprintf(stderr, "Erreur : syntaxe invalide après 'for'.\n");
        *status = 1;
        return;
    }
    ptr += 3; // Sauter "in "

    // Lire le répertoire cible
    char dir_path[MAX_PATH_LEN];
    i = 0;
    while (*ptr != ' ' && *ptr != '{' && *ptr != '\0') {
        dir_path[i++] = *ptr++;
    }
    dir_path[i] = '\0';

    // Vérifier la présence de '{'
    while (*ptr == ' ') ptr++;
    if (*ptr != '{') {
        fprintf(stderr, "Erreur : '{' attendu après le chemin du répertoire.\n");
        *status = 1;
        return;
    }
    ptr++;

    // Lire la commande dans les accolades
    char command_template[256];
    i = 0;
    while (*ptr != '}' && *ptr != '\0') {
        command_template[i++] = *ptr++;
    }
    command_template[i] = '\0';

    if (*ptr != '}') {
        fprintf(stderr, "Erreur : '}' attendu à la fin de la commande.\n");
        *status = 1;
        return;
    }

    // Vérifier que la commande contient la variable
    char var_with_dollar[64];
    snprintf(var_with_dollar, sizeof(var_with_dollar), "$%s", variable);
    if (!contient_variable(command_template, var_with_dollar)) {
        fprintf(stderr, "Erreur : la commande doit contenir la variable $%s.\n", variable);
        *status = 1;
        return;
    }

    // Exécuter la traversée du répertoire
    traverser_repertoire(dir_path, command_template, var_with_dollar, status, 0, 0, '\0', NULL);
}
