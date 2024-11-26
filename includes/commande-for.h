#ifndef FOR_COMMAND_H
#define FOR_COMMAND_H

int is_internal_command(const char *cmd);
void execute_command(const char *cmd_template, const char *filename, int *status);
void process_fichier(const char *path, const char *ext, char type, int include_hid, const char *cmd_template, int *status);
void traverser_repertoir(const char *dir_path, const char *cmd_template, int *status, int include_hid, int recursive, const char *ext, char type);
void execute_for(const char *ligne, int *status);

#endif
