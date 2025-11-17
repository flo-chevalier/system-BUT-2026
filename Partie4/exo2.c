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
    int nombre = 4;

    switch (pidEnfant = fork()) {
        case -1:
            erreur("fork");
        case 0:
            nombre = 2;
            printf("[enfant] PID = %d \t x=%d \t &x=%p\n", getpid(), nombre, &nombre);
            exit(EXIT_SUCCESS); // on kill l'enfant
        default:
            printf("[parent] PID = %d \t x=%d \t &x=%p\n", getpid(), nombre, &nombre);
            break;
    }

    // à partir d'ici, il ne reste que le père
    waitpid(pidEnfant, NULL, 0);

    printf("\nParent : enfant terminé normalement!\n");

    return EXIT_SUCCESS;
}
