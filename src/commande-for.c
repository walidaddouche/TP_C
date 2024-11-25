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

void execute_command(const char *cmd_template, const char *filename, int *status) {
     if (strncmp(cmd_template, "ftype ", 6) == 0) {
         ftype(filename,status);
    }
}


void process_fichier(const char *path, const char *ext, char type, int include_hid, const char *cmd_template, int *status) {
    struct stat st;
    if (stat(path, &st) == -1) return;

    
    if ((type == 'f' && !S_ISREG(st.st_mode)) ||
        (type == 'd' && !S_ISDIR(st.st_mode)) ||
        (type == 'l' && !S_ISLNK(st.st_mode)) ||
        (type == 'p' && !S_ISFIFO(st.st_mode))) return;

    
    if (ext) {
        const char *file_ext = strrchr(path, '.');
        if (!file_ext || strcmp(file_ext + 1, ext) != 0) return;
    }

    
    execute_command(cmd_template, path, status);
}

void traverser_repertoir(const char *dir_path, const char *cmd_template, int *status, int include_hid, int recursive, const char *ext, char type) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (!include_hid && entry->d_name[0] == '.') continue;
        char path[MAX_PATH_LEN];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (entry->d_type == DT_DIR && recursive) {
            traverser_repertoir(path, cmd_template, status, include_hid, recursive, ext, type);
        } else {
            process_fichier(path, ext, type, include_hid, cmd_template, status);
        }
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
    int include_hid = 0;  
    int recursive = 0;    
    const char *ext = NULL;  
    char type = 0;  
    while (*ptr == '-') {
        if (strncmp(ptr, "-A", 2) == 0) {
            include_hid = 1;
            ptr += 2;
        } else if (strncmp(ptr, "-r", 2) == 0) {
            recursive = 1;
            ptr += 2;
        } else if (strncmp(ptr, "-e", 2) == 0) {
            ptr += 2;
            while (*ptr == ' ') ptr++;  
            ext = ptr;
            while (*ptr != ' ' && *ptr != '{' && *ptr != '\0') ptr++;  
        } else if (strncmp(ptr, "-t", 2) == 0) {
            ptr += 2;
            while (*ptr == ' ') ptr++;  
            type = *ptr++;
        } else {
            break;  
        }
    }
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
         
    traverser_repertoir(rep, cmd, status, include_hid, recursive, ext, type);
}

