#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 5
#define MAX 100

void erreur(const char* message) {
    printf("Erreur durant : %s\n", message);
    exit(EXIT_FAILURE);
}

void ouvrirTube(int tube[]) {
    if (pipe(tube) == -1) {
        erreur("ouverture tube");
    }
}

int main(int argc, char const* argv[]) {
    int tubeVersEnfant[2];
    int tubeVersPere[2];
    char tampon[MAX];

    ouvrirTube(tubeVersEnfant);
    ouvrirTube(tubeVersPere);

    int pid = fork();

    if (pid == -1) {
        erreur("fork");
    }
    else if (pid == 0) {
        //enfant
        close(tubeVersEnfant[1]);
        close(tubeVersPere[0]);

        for (int i = 0; i < N; i++) {
            read(tubeVersEnfant[0], tampon, MAX);
            printf("Enfant, j'ai recu : %s\n", tampon);

            sleep(1);
            snprintf(tampon, MAX, "Pong %d", i);

            printf("Enfant, j'envoie %s\n", tampon);
            write(tubeVersPere[1], tampon, strlen(tampon) + 1);
        }

        close(tubeVersEnfant[0]);
        close(tubeVersPere[1]);

        exit(EXIT_SUCCESS);
    }
    else {
        //parent
        close(tubeVersEnfant[0]);
        close(tubeVersPere[1]);

        for (int i = 0; i < N; i++) {
            sleep(1);

            snprintf(tampon, MAX, "Ping %d", i);

            printf("Parent, j'envoie %s\n", tampon);
            write(tubeVersEnfant[1], tampon, strlen(tampon) + 1);

            read(tubeVersPere[0], tampon, MAX);
            printf("Parent, j'ai recu : %s\n\n", tampon);
        }

        wait(&pid);

        close(tubeVersEnfant[1]);
        close(tubeVersPere[0]);
    }

    return EXIT_SUCCESS;
}
