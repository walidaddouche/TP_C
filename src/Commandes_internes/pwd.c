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
