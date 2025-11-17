#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void erreur(const char* message) {
    printf("Erreur durant : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(const int argc, char const* argv[]) {
    int pidEnfant;

    switch (pidEnfant = fork()) {
        case -1:
            erreur("fork");
        case 0:
            printf("[enfant] Execution de la commande cat.\n");
            execlp("cat", "cat", "-n", "exo3.c", NULL);
            erreur("exec");
        default:
            break;
    }

    // à partir d'ici, il ne reste que le père
    waitpid(pidEnfant, NULL, 0);

    printf("\nParent : enfant terminé\n");

    return EXIT_SUCCESS;
}
