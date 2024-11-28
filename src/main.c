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

#define CWDSIZE 1024
void prompt(int status);
void trait_ligne_commande(char *ligne, int *status, char *previous_dir);

int main() {
    rl_outstream = stderr;
    char *ligne;
    int status = 0; 
    static char previous_dir[CWDSIZE] = "";


    while (1) {
        prompt(status);  
        ligne = readline("");  

        if (!ligne) {
            printf("\n");
            execution_exit(&status, NULL); 
            break;
        }
        if (*ligne) {
            add_history(ligne); 
            trait_ligne_commande(ligne,&status,previous_dir); 
        }
        free(ligne);
    }

    return 0;
}


