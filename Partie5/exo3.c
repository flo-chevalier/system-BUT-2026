#define _GNU_SOURCE
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 9876543210
#define NB 6

typedef struct {
    int numThread;
    uint64_t debut;
    uint64_t fin;
    uint64_t resultat;
} Arguments;


void afficherTemps() {
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    char buf[64];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &tm);
    printf("%s\n", buf);
}

void* fonctionThread(void* arg) {
    Arguments* arguments = (Arguments*) arg;
    uint64_t somme = 0;

    printf("[thread %d] calcul de %zu à %zu\n", arguments->numThread, arguments->debut, arguments->fin);

    for (uint64_t i = arguments->debut; i <= arguments->fin; i++) {
        somme += i;
    }

    arguments->resultat = somme;
    pthread_exit(NULL);
}


int main() {
    pthread_t thread[NB];
    Arguments donnees[NB];
    uint64_t intervalle = N / NB;
    uint64_t resultat = 0;

    printf("[main] nb threads=%d\n", NB);
    afficherTemps();


    for (int i = 0; i < NB; i++) {
        donnees[i].numThread = i;
        donnees[i].debut = intervalle * i + 1;
        donnees[i].fin = intervalle * (i + 1);

        if (i == NB - 1) {
            donnees[i].fin = N;
        }

        if (pthread_create(&thread[i], NULL, fonctionThread, &donnees[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NB; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }

        resultat += donnees[i].resultat;
    }

    printf("Resultat final = %zu\n", resultat);
    afficherTemps();

    return EXIT_SUCCESS;
}
