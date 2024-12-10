#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void execute_consecutive_cmd(const char *ligne, char **commands, int *num_cmds) {
    char *line_copy = strdup(ligne);
    char *token = strtok(line_copy, ";");
    *num_cmds = 0;

    while (token != NULL) {
        commands[*num_cmds] = strdup(token);
        (*num_cmds)++;
        token = strtok(NULL, ";");
    }

    free(line_copy);
}