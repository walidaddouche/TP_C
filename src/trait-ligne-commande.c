#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../includes/prompt.h"
#include "../includes/status.h"
#include "../includes/exit.h"
#include "../includes/pwd.h"
#include "../includes/ftype.h"
#include "../includes/cd.h"
#include"../includes/commande-for.h"
#include "../includes/cmd-externes.h"

#define MAX_ARGS 64
#define MAX_PATH_LEN 1024
// Fonction pour exécuter une boucle "for"
void execute_for(const char *ligne, int *status);
// Fonction pour découper une commande en argument
void parse_command(char *line, char **argv) {
    int i = 0;// Compteur pour les arguments
    char *token = strtok(line, " ");// Découpe la chaîne en mots
    while (token != NULL && i < MAX_ARGS - 1) {
  argv[i++] = token;// Ajoute chaque mot comme argument
        token = strtok(NULL, " ");// Passe au mot suivant
    }
    argv[i] = NULL;// Termine la liste d'arguments avec NULL
}
// Fonction pour traiter les lignes de commande
void trait_ligne_commande(char *ligne, int *status, char *previous_dir) {
    // Supprime le retour à la ligne à la fin de la commande
    ligne[strcspn(ligne, "\n")] = '\0';
    // Si l'utilisateur entre "exit", on quitte le shell
    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    }
    // Si "exit" est suivi d'un code de sortie
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;// Récupère le code après "exit "
        execution_exit(status, args);
    }
    // Si la commande est pwd, affiche le répertoire courant
    else if (strncmp(ligne, "pwd",3) == 0) {
        char *ptr;
        ptr = ligne + 3;// Passe les 3 caractères de "pwd"
        while (*ptr == ' ') ptr++;// Ignore les espaces
        if (*ptr != '\0') {//Si on trouve un arrgument (chaine de carctères)
           printf("erreur dans la syntaxe de pwd\n");
           *status=1;
           return;
        }
        execution_pwd();
  }
    else if (strncmp(ligne, "ftype", 5) == 0) {
       char ref[MAX_PATH_LEN], *ptr;
       int i = 0;
       ptr = ligne + 5;// Passe les 5 caractères de "ftype"
       while (*ptr == ' ') ptr++;
       if (*ptr == '\0') {//Si aucun argument fourni
          printf("erreur dans la syntaxe de ftype : argument manquant\n");
          *status = 1;
          return;
       }
       while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
          ref[i++] = *ptr++;// Récupère l'argument (chemin)
       }
       ref[i] = '\0';
       while (*ptr == ' ') ptr++;// Ignore les espaces restants
       if (*ptr != '\0') {// Si on trouve autre argument
          printf("erreur dans la syntaxe de ftype : argument malformé\n");
          *status = 1;
          return;
       }
       if (i == 0) {//Pour assurer qu'il y a un argument
          printf("erreur dans la syntaxe de ftype : référence vide\n");
          *status = 1;
          return;
       }
       ftype(ref, status);
   }

    else if (strncmp(ligne, "cd", 2) == 0) {
        char ref[MAX_PATH_LEN], *ptr;
        int i = 0;
        ptr = ligne + 2;
        while (*ptr == ' ') ptr++;
        if (*ptr == '\0') {cd(NULL, status, previous_dir);// cd sans argument 
        }
        else{
           while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
             ref[i++] = *ptr++;// Récupère l'argument
           }
           ref[i] = '\0';
           while (*ptr == ' ') ptr++;
           if (*ptr != '\0') {//Si on trouve autre argument
             printf("Erreur dans la syntaxe de cd : arguments malformés\n");
             *status = 1;
             return;
           }
           cd(ref, status, previous_dir);// Appelle la fonction pour gérer cd
       }
    }
     else if (strncmp(ligne, "for", 3) == 0) {
        execute_for(ligne,status);// Gère la boucle for
    }
  // Sinon, il s'agit d'une commande externe
 else {
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);// Découpe la ligne en arguments
        if (argv[0] != NULL) {
            cmd_externes(argv, status);// Exécute la commande externe
        }
    }
}
