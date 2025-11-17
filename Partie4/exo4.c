#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// indicateur qu'un signal est reçu
volatile sig_atomic_t stop = 0;

// Handler qui récupère le signal utilisé
void on_signal(const int signal) {
    // demande l'arrêt de la boucle d'attente
    if (signal == SIGTERM) {
        stop = 1;
    }

    printf("\nSignal reçu : %d (%s)\n", (int)signal, strsignal(signal));
}


int main(void) {
    struct sigaction sa = {0};

    // fonction à appeler à la réception du signal désiré
    sa.sa_handler = on_signal;

    // aucun signal supplémentaire masqué pendant le handler
    sigemptyset(&sa.sa_mask);

    // tenter de relancer certains appels bloquants
    sa.sa_flags = SA_RESTART;


    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    while (!stop) {
        printf("PID = %d. Appuyez sur Ctrl+C pour envoyer SIGINT.\n", getpid());
        sleep(1);
    }

    return EXIT_SUCCESS;
}
