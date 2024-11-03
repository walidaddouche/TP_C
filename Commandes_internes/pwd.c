#define Max 1024

void print_prompt() {
    printf("$ ");
}

void execution_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd error");
    }
}



void analyse_and_execute(char *cmd) {
    cmd[strcspn(cmd, "\n")] = '\0';
    if (strcmp(cmd, "pwd") == 0) {
        execution_pwd();
    } else if (strcmp(cmd, "exit") == 0) {
        exit(0);
    } else {
        printf("Commande introuvable : %s\n", cmd);
    }
}



int main() {
    char cmd[Max];
    while (1) {
        print_prompt();  // Affiche le prompt "$ "
        if (fgets(cmd, Max, stdin) == NULL) {
            break;
        }
        analyse_and_execute(cmd);
    }
    return 0;
}