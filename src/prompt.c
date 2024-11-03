#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define VERT "\001\033[32m\002"
#define RED "\001\033[91m\002"
#define BLEU "\001\033[36m\002"
#define NORMAL "\001\033[00m\002"
#define CWDSIZE 1024

void prompt(int status) {
    char path[CWDSIZE];
    char *color_status = (status == 0) ? VERT : RED;
    char display_status[10]; // Chaîne pour stocker le statut d'affichage

    // Définir display_status selon le code de sortie ou si un signal a été reçu
    if (status == 0) {
        snprintf(display_status, sizeof(display_status), "0");
    } else if (status >= 128) {
        snprintf(display_status, sizeof(display_status), "SIG");
    } else {
        snprintf(display_status, sizeof(display_status), "%d", status);
    }

    // Obtenir le répertoire actuel
    if (getcwd(path, sizeof(path)) == NULL) {
        perror("prompt");
        return;
    }

    // Afficher le prompt avec la couleur et le statut
    printf("%s[%s]%s", color_status, display_status, BLEU);

    // Gestion du répertoire (affichage de la fin si trop long)
    if (strlen(path) > 20) {
        printf("...%s", path + strlen(path) - 20);
    } else {
        printf("%s", path);
    }

    printf(NORMAL "$ ");
    fflush(stdout);
}
