/*
 p2e3.c - I/O fichier (libc/stdio)
 Écrit un texte dans anneau.txt avec fopen/fputs puis le lit ligne par ligne
 avec fgets.
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *fname = "anneau.txt";
    FILE *f = fopen(fname, "w");
    if (!f) {
        perror("fopen for write");
        return 1;
    }

    /* Écriture : on utilise fprintf pour plus de clarté */
    fprintf(f, "Un Anneau pour les gouverner tous,\n");
    fprintf(f, "un Anneau pour les trouver,\n");
    fprintf(f, "un Anneau pour les amener tous et dans les ténèbres les lier.\n");

    if (fclose(f) == EOF) {
        perror("fclose after write");
        return 1;
    }

    /* Lecture ligne par ligne avec fgets */
    f = fopen(fname, "r");
    if (!f) {
        perror("fopen for read");
        return 1;
    }

    char buf[512];
    while (fgets(buf, sizeof(buf), f)) {
        /* fgets inclut le '\n' si présent */
        fputs(buf, stdout);
    }

    if (ferror(f)) {
        fprintf(stderr, "Erreur de lecture sur %s\n", fname);
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}
