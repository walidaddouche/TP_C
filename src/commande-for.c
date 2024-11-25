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
 void execute_internal_command(const char *cmd, const char *filename, const struct stat *st, int *status) {
    if (S_ISREG(st->st_mode)) {  
        if (strcmp(cmd, "ftype",5) == 0) {
            ftype(filename, &status);

        }
    } else if (S_ISDIR(st->st_mode)) { 
        if (strcmp(cmd, "ftype",5) == 0) {
            ftype(filename, &status);
        } else if (strcmp(cmd, "cd",2) == 0) {
             char *path = cmd + 2;
             while (*path == ' ') path++; 
             if (*path == '\0') {
                path = NULL; 
             }
             cd(path,&status, previous_dir);

        } else if (strcmp(cmd, "pwd",3) == 0) {
             execution_pwd();
        }
    } else if (S_ISLNK(st->st_mode)) { 
        if (strcmp(cmd, "ftype",5) == 0) {
            ftype(filename, &status);
        }
    } else {  
        if (strcmp(cmd, "ftype",5) == 0) {
            ftype(filename, &status);
        }
    }
}

void execute_external_command(const char *cmd, const char *filename, int *status) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(cmd, cmd, filename, NULL);
        perror("Erreur d'exécution de la commande externe");
        *status=1
        return;
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("Erreur lors du fork");
    }
}

// Déterminer si la commande est interne ou externe, puis l'exécuter
void execute_command(const char *cmd_template, const char *filename, const struct stat *st,int *status) {
    if (strcmp(cmd, "ftype",5) == 0 || strcmp(cmd, "cd",2) == 0 || strcmp(cmd, "pwd",3) == 0) {
        execute_internal_command(cmd, filename, st,status);
    } else {
        execute_external_command(cmd, filename,status);
    }
}


void process_fichier(const char *path, const char *cmd_template, int *status) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("Erreur de lstat");
        *status=1;
        return;
    }
    
    execute_command(cmd_template, path, status);
}

void traverser_repertoir(const char *dir_path, const char *cmd_template, int *status) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        *status=1;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, ".." || entry->d_name[0] == '.') == 0) continue;
        char path[MAX_PATH_LEN];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);    
        process_fichier(path, cmd_template, status);
        
    }
    closedir(dir);
}

void execute_for(const char *ligne, int *status) {
    char *ptr = ligne + 3;  

    while (*ptr == ' ') ptr++;  
    while (*ptr != ' ' && *ptr != '\0') ptr++;
    while (*ptr == ' ') ptr++; 
    if (strncmp(ptr, "in", 2) != 0) {
        fprintf(stderr, "Erreur : syntaxe invalide, 'in' attendu.\n");
        return;
    }
    ptr += 2;  
    while (*ptr == ' ') ptr++; 
    char rep[256];
    int i = 0;
    while (*ptr != ' ' && *ptr != '{' && *ptr != '\0') {
        rep[i++] = *ptr++;
    }
    rep[i] = '\0';  
    while (*ptr == ' ') ptr++;  
 
    if (*ptr != '{') {
        fprintf(stderr, "Erreur : syntaxe invalide, '{' attendu.\n");
        return;  
    }
    ptr++;  
    while (*ptr == ' ') ptr++;  
    char cmd[256];
    i = 0;
    while (*ptr != '$'  ) {
        cmd[i++] = *ptr++;
    }
    cmd[i] = '\0';
    while (*ptr != '}' && *ptr != '\0'  ) {
        *ptr++;
    }
    if (*ptr != '}') {
        fprintf(stderr, "Erreur : syntaxe invalide, '}' attendu.\n");
        return;
   }
         
    traverser_repertoir(rep, cmd, status);
}

