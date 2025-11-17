// p3e5.c — Écriture via pointeur non initialisé (bug mémoire démonstratif)
// Compile: gcc -std=c2x -Wall -Wextra -pedantic -g p3e5.c -o p3e5
// Expect: segmentation fault / Invalid write (valgrind/asan)
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

int main(void) {
    int *p;  // non initialisé -> indéfini
    *p = 42; // BUG : écriture à une adresse indéterminée
    printf("*p = %d\n", *p);
    return 0;
}
