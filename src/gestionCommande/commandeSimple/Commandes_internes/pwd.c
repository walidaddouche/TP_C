#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

void execution_pwd() {
    char cwd[1024];//taille du tampon
    if (getcwd(cwd, sizeof(cwd)) != NULL) {//si getcwd reussit 
        printf("%s\n", cwd);//afficher le chemin du repertoir courant et une nouvelle ligne 
    } else {
        perror("pwd error");//message d'erreur 
    }
}
