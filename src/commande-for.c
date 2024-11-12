#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void execute_command(const char *cmd_template, const char *filename) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), cmd_template, filename);
    system(cmd);  
}

void process_file(const char *path, const char *ext, char type, int include_hidden, const char *cmd_template) {
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
    execute_command(cmd_template, path);
}

void traverse_directory(const char *dir_path, const char *ext, char type, int include_hidden, int recursive, const char *cmd_template) {
    DIR *dir = opendir(dir_path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (!include_hidden && entry->d_name[0] == '.') continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR && recursive) {
            traverse_directory(path, ext, type, include_hidden, recursive, cmd_template);
        } else {
            process_file(path, ext, type, include_hidden, cmd_template);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *dir_path = ".";
    const char *ext = NULL;
    char type = 'f';
    int include_hidden = 0;
    int recursive = 0;
    const char *cmd_template = "echo %s"; 
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-A") == 0) {
            include_hidden = 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            ext = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            type = argv[++i][0];
        } else if (strcmp(argv[i], "{") == 0 && i + 1 < argc) {
            cmd_template = argv[++i];
            if (i + 1 < argc && strcmp(argv[i + 1], "}") == 0) i++;
        } else {
            dir_path = argv[i];
        }
    }
    traverse_directory(dir_path, ext, type, include_hidden, recursive, cmd_template);

    return 0;
}