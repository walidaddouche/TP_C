#include <stdio.h>      // Pour printf et perror
#include <unistd.h>     // Pour getcwd
#include <stddef.h>     // Pour NULL (optionnel, car déjà inclus par <stdio.h>)

void execution_pwd() {
    char cwd[1024];
    
    // Vérifier si getcwd réussit
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);  // Afficher le répertoire actuel
    } else {
        perror("pwd error");  // Afficher l'erreur en cas d'échec
    }
}

