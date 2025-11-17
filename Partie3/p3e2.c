// p3e2.c — Croissance du tas (heap) par fuites volontaires
// Compile: gcc -std=c2x -Wall -Wextra -pedantic -g p3e2.c -o p3e2
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const size_t block = 1024 * 1024; // 1 MiB
    // Très longue boucle
    for (unsigned long long i = 0; i < 1000000000000ULL; ++i) {
        void *p = malloc(block);
        if (!p) {
            fprintf(
                stderr, "malloc failed at i=%llu: %s\n", i, strerror(errno));
            return 1;
        }
        printf("i=%llu, ptr=%p\n", i, p);

        // Intentionnellement pas de free -> fuite pour faire grossir la heap
    }
    return 0;
}
