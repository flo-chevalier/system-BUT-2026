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

    printf("Avant fork\n");
    printf("PID = %d\n\n", getpid());

    switch (pidEnfant = fork()) {
        case -1:
            erreur("fork");
        case 0:
            printf("Je suis l'enfant : PID = %d. Mon père est : PID = %d\n", getpid(), getppid());
            exit(EXIT_SUCCESS); // on kill l'enfant
        default:
            printf("Je suis le père : PID = %d. Mon enfant est : PID = %d\n", getpid(), pidEnfant);
            break;
    }

    // à partir d'ici, il ne reste que le père
    waitpid(pidEnfant, NULL, 0);

    printf("\nParent : enfant terminé normalement!\n");

    return EXIT_SUCCESS;
}
