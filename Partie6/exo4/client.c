#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define ADRESSE "127.0.0.1"

void lireMessage(char tampon[]) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int main() {
    char message[MAX_BUFFER];

    int fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // 2. Configuration de l'adresse du serveur
    struct sockaddr_in coordonneesServeur = {0};
    coordonneesServeur.sin_family = AF_INET; // de type TCP
    coordonneesServeur.sin_port = htons(PORT); // le port d'écoute du serveur
    inet_aton(ADRESSE, &coordonneesServeur.sin_addr); // adresse du serveur

    // 3. Connexion au serveur
    printf("[Client TCP] Connexion au serveur %s:%d...\n", ADRESSE, PORT);
    if (connect(fdSocket, (struct sockaddr*) &coordonneesServeur, sizeof coordonneesServeur) == -1) {
        perror("connect");
        close(fdSocket);
        exit(EXIT_FAILURE);
    }

    printf("[Client TCP] Connecté !\n");

    printf("[Client TCP] Saisir votre pseudo :\n");
    scanf("%s", message);
    send(fdSocket, message, strlen(message), 0);

    printf("\n[Client] Entrez vos messages (exit pour quitter) :\n");

    while (1) {
        lireMessage(message);

        printf("\033[1A"); // remonte d’une ligne
        printf("\033[2K"); // efface la ligne

        // on envoie le message au serveur
        send(fdSocket, message, strlen(message), 0);

        memset(message, 0, MAX_BUFFER);
        // on attend la réponse du serveur
        int nbRecu = recv(fdSocket, message, MAX_BUFFER, 0);

        if (nbRecu == 0) {
            printf("[Client] Serveur déconnecté\n");
            break;
        }

        if (nbRecu > 0) {
            message[nbRecu] = 0;
            printf("[Client] Recu : %s\n", message);
        }
    }

    printf("\n[Client] Fermeture de la connexion...\n");
    shutdown(fdSocket, SHUT_WR); // On n'envoie plus

    close(fdSocket);

    return EXIT_SUCCESS;
}
