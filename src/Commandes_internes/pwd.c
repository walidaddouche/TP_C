<<<<<<< HEAD
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

void execution_pwd() {
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd error");
    }
}

=======
#include <stdio.h>      
#include <unistd.h>     
#include <stddef.h>     

void execution_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);  
    } else {
        perror("pwd error");
    }
}


>>>>>>> e67dbdea38ba945b7cb9ea80409d86bc10e26c31
