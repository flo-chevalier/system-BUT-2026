#define _GNU_SOURCE
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int valeur;
    const char* message;
} Arguments;

void* fonctionThread(void* arg) {
    Arguments* arguments = (Arguments*) arg;
    //unsigned long tidDuThread = gettid();
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);

    printf("[thread\t PID: %d - TID: %" PRIu64 "] value : %d , message : %s\n", getpid(), tid, arguments->valeur, arguments->message);

    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    //unsigned long tidDuMain = gettid();
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    Arguments donnees = {.valeur = 5, .message = "coucou"};

    printf("[main\t PID: %d - TID: %" PRIu64 "] création du thread...\n", getpid(), tid);

    if (pthread_create(&thread, NULL, fonctionThread, &donnees) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    printf("[main\t PID: %d - TID: %" PRIu64 "] fin du thread\n", getpid(), tid);

    return EXIT_SUCCESS;
}
