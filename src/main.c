#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "../includes/prompt.h"
#include "../includes/status.h"
#include"../includes/exit.h"
#include"../includes/trait-ligne-commande.h"

#define CWDSIZE 1024// taille max du chemin courant 
void prompt(int status);
void trait_ligne_commande(char *ligne, int *status, char *previous_dir);

int main() {
    rl_outstream = stderr;//configure la sortie de readline vers stderr
    char *ligne;//pour stocker la ligne de commande saisie
    int status = 0; //pour stocker le statut de la derniere commande executer 
    static char previous_dir[CWDSIZE] = "";//tampon pour le dernier repertoir qu'on a visiter 


    while (1) {
        prompt(status); ///affiche le statut de la derniere commande et le prompt personaliser
        ligne = readline("");  //lire une ligne saisie 

        if (!ligne) { //si l'utilisateur envoi un signal  ctrl-D 
            printf("\n");
            execution_exit(&status, NULL); 
            break;
        }
        if (*ligne) {//si la ligne n'est pas vide 
            add_history(ligne); //ajouter la commande a l'historique de readline 
            trait_ligne_commande(ligne,&status,previous_dir); //traiter la commande saisie
        }
        free(ligne);//liberer la memoir alouer pour la ligne de commande 
    }

    return 0;// pour indiquer la fin du programe avec succes
}


