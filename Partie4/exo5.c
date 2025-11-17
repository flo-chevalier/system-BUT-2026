#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAILLE 1000

void erreur(const char* message);
void ouvrirTube(int tube[]);
void fermerTube(int tube[]);
void ecrireDansTube(const int tube[]);
void lireDansTube(const int tube[], char* buffer);


int main(const int argc, char const* argv[]) {
    int enfant;
    int tubeVersEnfant[2];
    char message[TAILLE];

    ouvrirTube(tubeVersEnfant);

    /***** le code de l'enfant *****/
    switch (enfant = fork()) {
        case -1:
            printf("Erreur fork. Arret du programme.\n");
            exit(EXIT_FAILURE);
        case 0:
            lireDansTube(tubeVersEnfant, &message);
            printf("Enfant a recu le message : %s\n", message);

            fermerTube(tubeVersEnfant);
            return EXIT_SUCCESS;
        default:
            break;
    }

    /******** le code du parent ********/
    ecrireDansTube(tubeVersEnfant);

    waitpid(enfant, NULL, 0);

    fermerTube(tubeVersEnfant);

    printf("Je suis le père. Fin du programme\n");

    return EXIT_SUCCESS;
}

void erreur(const char* message) {
    printf("Erreur durant : %s\n", message);
    exit(EXIT_FAILURE);
}

void ouvrirTube(int tube[]) {
    if (pipe(tube) == -1) {
        erreur("ouverture tube");
    }
}

void fermerTube(int tube[]) {
    if (close(tube[0]) == -1) {
        erreur("fermeture tube 1");
    }
    if (close(tube[1]) == -1) {
        erreur("fermeture tube 2");
    }
}

void ecrireDansTube(const int tube[]) {
    char* message = "Ceci est un message";

    if (write(tube[1], message, strlen(message)) < 0) {
        erreur("écriture dans tube");
    }
}

void lireDansTube(const int tube[], char* buffer) {
    if (read(tube[0], buffer, TAILLE) == -1) {
        erreur("lecture dans tube");
    }
}
