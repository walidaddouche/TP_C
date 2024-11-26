#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include"../includes/ftype.h"

#define MAX_PATH_LEN 1024
#define MAX_ARGS 256

void execute_for(const char *ligne, int *status) {
    
    const char *ptr = ligne + 4; 
    while (*ptr == ' ') ptr++; 

    
    char variable[64];
    int i = 0;
    while (*ptr != ' ' && *ptr != '\0') {
        variable[i++] = *ptr++;
    }
    variable[i] = '\0';

    while (*ptr == ' ') ptr++;

    
    if (strncmp(ptr, "in", 2) != 0) {
        fprintf(stderr, "Erreur : syntaxe invalide, 'in' attendu.\n");
        *status = 1;
        return;
    }
    ptr += 2;
    while (*ptr == ' ') ptr++;

    char rep[MAX_PATH_LEN];
    i = 0;
    while (*ptr != ' ' && *ptr != '{' && *ptr != '\0') {
        rep[i++] = *ptr++;
    }
    rep[i] = '\0';

    while (*ptr == ' ') ptr++;

    
    if (*ptr != '{') {
        fprintf(stderr, "Erreur : syntaxe invalide, '{' attendu.\n");
        *status = 1;
        return;
    }
    ptr++;
    while (*ptr == ' ') ptr++;

    char command_template[256];
    i = 0;
    int quite = 0;
    while (!quite) {
        if ((*ptr == ' ' &&  *(ptr + 1)== '$' )|| *ptr == '\0') quite = 1;
        else command_template[i++] = *ptr++;
    }
    command_template[i] = '\0';


    while (*ptr == ' ') ptr++;

     if (*ptr != '$') {
        fprintf(stderr, "Erreur : syntaxe invalide, '$' attendu.\n");
        *status = 1;
        return;
    }
    ptr++; 
     if (*ptr != 'F') {
        fprintf(stderr, "Erreur : syntaxe invalide, 'F' attendu.\n");
        *status = 1;
        return;
    } 
    ptr++; 
    while (*ptr == ' ') ptr++;   

    if (*ptr != '}') {
        fprintf(stderr, "Erreur : syntaxe invalide, '}' attendu.\n");
        *status = 1;
        return;
    }

    traverser_repertoir(rep, command_template, variable, status);
}


void traverser_repertoir(const char *dir_path, const char *cmd_template, const char *variable, int *status) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        *status = 1;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
            continue;

        char path[MAX_PATH_LEN];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        char command[512];
        snprintf(command, sizeof(command), "%s %s", cmd_template, path);
        trait_ligne_commande(command, status, NULL);
    }
    closedir(dir);
}

