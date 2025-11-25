#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NB 4

// variables globales
int compteur = 0;
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;

void* incremente(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&verrou);
        compteur++;
        pthread_mutex_unlock(&verrou);
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

    pthread_mutex_destroy(&verrou);
    printf("Résultat final = %d\n", compteur);

    return EXIT_SUCCESS;
}
