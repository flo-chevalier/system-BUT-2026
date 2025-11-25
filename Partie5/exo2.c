#define _GNU_SOURCE
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    uint64_t n;
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

    for (uint64_t i = 0; i <= arguments->n; i++) {
        somme += i;
    }

    uint64_t* resultat = malloc(sizeof(uint64_t));
    if (!resultat) {
        pthread_exit(NULL);
    }
    *resultat = somme;

    pthread_exit(resultat);
}


int main() {
    Arguments donnees = {.n = 9876543210};
    pthread_t thread;

    afficherTemps();

    if (pthread_create(&thread, NULL, fonctionThread, &donnees) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    void* retour;
    if (pthread_join(thread, &retour) != 0) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    printf("Resultat = %zu\n", *(u_int64_t*) retour);
    afficherTemps();

    free(retour);

    return EXIT_SUCCESS;
}
