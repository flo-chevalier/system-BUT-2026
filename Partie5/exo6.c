#define _GNU_SOURCE
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define NB 4

atomic_int compteur = 0; // variable globale

void* incremente(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        atomic_fetch_add(&compteur, 1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NB];

    for (int i = 0; i < NB; i++) {
        pthread_create(&threads[i], NULL, incremente, NULL);
    }

    for (int i = 0; i < NB; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Résultat final = %d\n", compteur);

    return EXIT_SUCCESS;
}
