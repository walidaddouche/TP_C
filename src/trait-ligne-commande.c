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
//fonction pour exécuter une boucle "for"
void execute_for(const char *ligne, int *status);
//fonction pour découper une commande en argument
void parse_command(char *line, char **argv) {
    int i = 0;//compteur pour les arguments
    char *token = strtok(line, " ");//découpe la chaîne en mots
    while (token != NULL && i < MAX_ARGS - 1) {
  argv[i++] = token;//ajouter chaque mot comme argument
        token = strtok(NULL, " ");//passer au mot suivant
    }
    argv[i] = NULL;//terminer la liste d'arguments avec NULL
}
//fonction pour traiter les lignes de commande
void trait_ligne_commande(char *ligne, int *status, char *previous_dir) {
    //supprimer le retour à la ligne à la fin de la commande
    ligne[strcspn(ligne, "\n")] = '\0';
    //si l'utilisateur entre "exit", on quitte le shell
    if (strcmp(ligne, "exit") == 0) {
        execution_exit(status, NULL);
    }
    //si "exit" est suivi d'un code de sortie
    else if (strncmp(ligne, "exit ", 5) == 0) {
        char *args = ligne + 5;//récupère le code après "exit "
        execution_exit(status, args);
    }
    //si la commande est pwd, afficher le répertoire courant
    else if (strncmp(ligne, "pwd",3) == 0) {
        char *ptr;
        ptr = ligne + 3;//passer les 3 caractères de "pwd"
        while (*ptr == ' ') ptr++;//ignorer les espaces
        if (*ptr != '\0') {//si on trouve un argument (chaine de carctères)
           printf("erreur dans la syntaxe de pwd\n");
           *status=1;
           return;
        }
        execution_pwd();
  }
    else if (strncmp(ligne, "ftype", 5) == 0) {
       char ref[MAX_PATH_LEN], *ptr;
       int i = 0;
       ptr = ligne + 5;//passer les 5 caractères de "ftype"
       while (*ptr == ' ') ptr++;
       if (*ptr == '\0') {//si aucun argument est fourni
          printf("erreur dans la syntaxe de ftype : argument manquant\n");
          *status = 1;
          return;
       }
       while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
          ref[i++] = *ptr++;//récupère l'argument (chemin)
       }
       ref[i] = '\0';
       while (*ptr == ' ') ptr++;//ignorer les espaces restants
       if (*ptr != '\0') {//si on trouve  un autre argument
          printf("erreur dans la syntaxe de ftype : argument malformé\n");
          *status = 1;
          return;
       }
       if (i == 0) {//pour assurer qu'il y a un argument
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
        if (*ptr == '\0') {cd(NULL, status, previous_dir);//cd sans argument 
        }
        else{
           while (*ptr != ' ' && *ptr != '\0' && i < MAX_PATH_LEN - 1) {
             ref[i++] = *ptr++;//récupère l'argument
           }
           ref[i] = '\0';
           while (*ptr == ' ') ptr++;
           if (*ptr != '\0') {//si on trouve autre argument
             printf("Erreur dans la syntaxe de cd : arguments malformés\n");
             *status = 1;
             return;
           }
           cd(ref, status, previous_dir);//appelle la fonction pour gérer cd
       }
    }
     else if (strncmp(ligne, "for", 3) == 0) {
        execute_for(ligne,status);//gère la boucle for
    }
  //sinon il s'agit d'une commande externe
 else {
        char *argv[MAX_ARGS];
        parse_command(ligne, argv);//découper la ligne en arguments
        if (argv[0] != NULL) {
            cmd_externes(argv, status);//exécuter la commande externe
        }
    }
}
