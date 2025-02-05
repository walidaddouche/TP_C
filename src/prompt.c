#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
//code couleur pour l'affichage du terminal
#define VERT "\001\033[32m\002"// en cas de succee c'est vert 
#define RED "\001\033[91m\002"//rouge si il y'a une erreur 
#define BLEU "\001\033[36m\002"//le chemin est bleu
#define NORMAL "\001\033[00m\002"//pour réenisialiser la couleur 
#define CWDSIZE 1024// taille max du tampon pour le chemin

void prompt(int status) {
    char *path = malloc(CWDSIZE);//allocation de mémoir pour le chemin courant 
    if (!path) {//si l'allocation echou
        perror("malloc");//message d'érreur 
        return;
    }

    char *color_status = (status == 0) ? VERT : RED;//choisir une couleur par apport au statut
    char display_status[50]; //tampon

    if (status == 0) {
        snprintf(display_status, sizeof(display_status), "0"); //statut=0 donc succés
    } else if (status >= 128) {
        snprintf(display_status, sizeof(display_status), "SIG(%s)", strsignal(status - 128));//statut>=128 donc signal
    } else {
        snprintf(display_status, sizeof(display_status), "%d", status);//pour un autre statut on affiche le code numérique 
    }

    if (getcwd(path, CWDSIZE) == NULL) {//récuperer le chemin du répertoir courant 
        perror("getcwd");//erreur si getcwd echoue 
        free(path); //liberer la mémoire  
        return;
    }

    fprintf(stderr, "%s[%s]%s", color_status, display_status, BLEU);//afficher le statut en couleur qui corrépond et le chemin en bleu

    if (strlen(path) > 22) {//si le chemin est trop long on affiche seulement  les 22 derniers caractéres 
        fprintf(stderr, "...%s", path + strlen(path) - 22);//tronquer le chemin 
    } else {
        fprintf(stderr, "%s", path);//afficher le chemin complet
    }

    fprintf(stderr, NORMAL "$ ");//ajouter le symbole "$" a la fin et réinisialiser la couleur
    fflush(stderr);//forcer l'affichage du prompt
    free(path); //libérer la mémoire allouer pour le chemin
}
