#define _GNU_SOURCE
#include <ctype.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define BACKLOG 5
#define MAX_CLIENTS 10
#define DECALAGE 6

#define EXIT "/exit"
#define PING "/ping"
#define TIME "/time"
#define UPPER "/upper"
#define LOWER "/lower"


void lireMessage(char tampon[]) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}

int testPing(char tampon[]) {
    return strcmp(tampon, PING) == 0;
}

int testTime(char tampon[]) {
    return strcmp(tampon, TIME) == 0;
}

int testUpper(char tampon[]) {
    return tampon && strncmp(tampon, UPPER, DECALAGE) == 0;
}

int testLower(char tampon[]) {
    return tampon && strncmp(tampon, LOWER, DECALAGE) == 0;
}

void getDate(char tampon[]) {
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    // Format : "YYYY-MM-DD HH:MM:SS"
    strftime(tampon, 64, "%Y-%m-%d %H:%M:%S", &tm);
}

void stringToUpper(char *s) {
    if (!s) return;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        /* toupper prend un int convertible en unsigned char ou EOF */
        s[i] = (char) toupper((unsigned char) s[i]);
    }
}

void stringToLower(char *s) {
    if (!s) return;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        /* tolower prend un int convertible en unsigned char ou EOF */
        s[i] = (char) tolower((unsigned char) s[i]);
    }
}

void substring(const char *source, char *destination, int start, int length) {
    // Vérifiez si les indices sont valides
    if (start < 0 || length < 0 || start + length > strlen(source)) {
        printf("Indices invalides\n");
        return;
    }

    // Copiez la sous-chaîne
    strncpy(destination, source + start, length);
    // Assurez-vous que la chaîne de destination est null-terminée
    destination[length] = '\0';
}


int main() {
    char message[MAX_BUFFER];
    char messageEnvoi[MAX_BUFFER];

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

    printf("Client connecté - %s:%d\n",
               inet_ntoa(coordonneesClient.sin_addr),
               ntohs(coordonneesClient.sin_port));

    while (1) {
        // memset met tous les bits qui commencent à l'adresse de buffer à 0 pour MAX_BUFFER octets
        memset(message, 0, MAX_BUFFER);
        memset(messageEnvoi, 0, MAX_BUFFER);

        // on attend le message du client
        // la fonction recv est bloquante
        int nbRecu = recv(socketCommunication, message, MAX_BUFFER, 0);

        if (nbRecu > 0) {
            message[nbRecu] = 0;
            printf("[Serveur] Reçu : %s\n", message);

            if (testQuitter(message)) {
                printf("[Serveur] Client déconnecté\n");
                break; // on quitte la boucle
            }

            if (testPing(message)) {
                strcpy(messageEnvoi, "pong");
            } else if (testTime(message)) {
                getDate(messageEnvoi);
            } else if (testUpper(message)) {
                stringToUpper(message);
                substring(message, messageEnvoi, 6, strlen(message) - 6);
            } else if (testLower(message)) {
                stringToLower(message);
                substring(message, messageEnvoi, 6, strlen(message) - 6);
            } else {
                strcpy(messageEnvoi, message);
            }
        }

        // on envoie le message au client
        send(socketCommunication, messageEnvoi, strlen(messageEnvoi), 0);
        printf("[Serveur] Renvoyé : %s\n", messageEnvoi);
    }

    printf("\n[Serveur] Fermeture de la connexion...\n");
    shutdown(socketCommunication, SHUT_RDWR); // On n'envoie plus

    close(socketCommunication);
    close(socketAttente);

    return EXIT_SUCCESS;
}
