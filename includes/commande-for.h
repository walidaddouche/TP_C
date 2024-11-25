#ifndef FOR_COMMAND_H
#define FOR_COMMAND_H


// Définition des fonctions à déclarer dans l'en-tête

// Fonction pour vérifier si la commande est interne (cd, pwd, exit, ftype)
int is_internal_command(const char *cmd);

// Fonction pour exécuter la commande interne
void execute_command(const char *cmd_template, const char *filename, int *status);

// Fonction pour traiter un fichier dans le répertoire
void process_fichier(const char *path, const char *ext, char type, int include_hid, const char *cmd_template, int *status);

// Fonction pour parcourir un répertoire et exécuter une commande sur les fichiers
void traverser_repertoir(const char *dir_path, const char *cmd_template, int *status, int include_hid, int recursive, const char *ext, char type);

// Fonction pour exécuter la boucle for avec options
void execute_for(const char *ligne, int *status);

#endif // FOR_COMMAND_H
