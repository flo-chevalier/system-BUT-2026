#define _GNU_SOURCE
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define BACKLOG 5
#define MAX_CLIENTS 10


void lireMessage(char tampon[]) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int main() {
    char message[MAX_BUFFER];

    // 1. connexion TCP en mode connecté
    int socketAttente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketAttente == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Option SO_REUSEADDR pour éviter "Address already in use", uniquement côté serveur
    int opt = 1;
    if (setsockopt(socketAttente, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {
        perror("setsockopt");
        close(socketAttente);
        exit(EXIT_FAILURE);
    }

    // 2. Configuration de l'adresse (écoute sur toutes les interfaces, port 8080)
    struct sockaddr_in coordonneesServeur = {0};
    coordonneesServeur.sin_family = AF_INET;
    coordonneesServeur.sin_port = htons(PORT); // port 8080
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0 → écouter sur toutes les interfaces IPv4

    // 3. Bind
    if (bind(socketAttente, (struct sockaddr*) &coordonneesServeur, sizeof coordonneesServeur) == -1) {
        perror("bind");
        close(socketAttente);
        exit(EXIT_FAILURE);
    }

    // 5. Listen
    if (listen(socketAttente, BACKLOG) == -1) {
        perror("listen");
        close(socketAttente);
        exit(EXIT_FAILURE);
    }

    // 6. Acceptation d'un client (à faire dans une boucle pour en accepter plusieurs)
    struct sockaddr_in coordonneesClient;
    socklen_t tailleCoord = sizeof coordonneesClient;

    int socketCommunication = accept(socketAttente, (struct sockaddr*) &coordonneesClient, &tailleCoord);
    if (socketCommunication == -1) {
        perror("accept");
        close(socketAttente);
        exit(EXIT_FAILURE);
    }

    printf("Client connecté !\n");

    // on attend le message du client
    // la fonction recv est bloquante
    int nbRecu = recv(socketCommunication, message, MAX_BUFFER, 0);

    if (nbRecu > 0) {
        message[nbRecu] = 0;
        printf("Recu : %s\n", message);
    }

    // on envoie le message au client
    send(socketCommunication, message, strlen(message), 0);

    printf("\n[Client TCP] Fermeture de la connexion...\n");
    shutdown(socketCommunication, SHUT_RDWR); // On n'envoie plus

    close(socketCommunication);
    close(socketAttente);

    return EXIT_SUCCESS;
}
