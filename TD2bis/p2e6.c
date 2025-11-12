/*
 p2e6.c - Renommer un fichier (libc)
 Usage: p2e6 OLD NEW
 Si NEW existe déjà, affiche une erreur et n'écrase pas.
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s OLD NEW\n", argv[0]);
        return 2;
    }
    const char *old = argv[1];
    const char *newp = argv[2];

    if (access(newp, F_OK) == 0) {
        fprintf(stderr, "Erreur: '%s' existe déjà. Abandon.\n", newp);
        return 1;
    }
    if (rename(old, newp) != 0) {
        perror("rename");
        return 1;
    }
    return 0;
}
