#ifndef FOR_LOOP_H
#define FOR_LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_hidden(const char *filename); 
int has_extension(const char *filename, const char *ext);
int check_file_type(const char *path, char type);
void list_files_recursive(const char *dir, int include_hidden, const char *ext, char type, char **argv, int *status);
void execute_for_loop(char *ligne, int *status);

#endif
