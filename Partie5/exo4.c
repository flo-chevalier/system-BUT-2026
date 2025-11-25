#define _GNU_SOURCE
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NB 4

typedef struct {
    int numThread;
    int sleep;
} Arguments;

void* fonctionThread(void* arg) {
    Arguments* argument = (Arguments*) arg;
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    //unsigned long tid = gettid();

    printf("thread %d (PID: %d - TID: %zu) va travailler %ds\n", argument->numThread, getpid(), tid, argument->sleep);
    sleep(argument->sleep);
    printf("thread %d (PID: %d - TID: %zu) travail de %ds terminé\n", argument->numThread, getpid(), tid, argument->sleep);

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    pthread_t threads[NB];
    Arguments donnees[NB];

    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    printf("[main] - PID: %d - TID: %zu - nb_threads: %d\n", getpid(), tid, NB);

    for (int i = 0; i < NB; i++) {
        donnees[i].numThread = i + 1;
        donnees[i].sleep = rand() % 4 + 1;

        if (pthread_create(&threads[i], NULL, fonctionThread, &donnees[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NB; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Fin du programme\n");

    return EXIT_SUCCESS;
}
