// Compile: gcc -std=c2x -Wall -Wextra -pedantic -g p3e6.c -o p3e6 && ./p3e6
// Change size with: #define N 1 (or larger)
#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef TAILLE
#define TAILLE 1
#endif

// Global initialised (data)
int g_init[TAILLE] = {1};

// Static uninitialised (bss)
int s_bss[TAILLE];

int main(void) {
    // Local (stack)
    int local[TAILLE];
    for (int i = 0; i < TAILLE; i++)
        local[i] = i;

    // Heap
    int *heap = (int *)malloc(TAILLE * sizeof(int));
    if (!heap) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < TAILLE; i++)
        heap[i] = i;

    printf("\n--- Adresses (pointers) ---\n");
    printf("code (text)   main   : %p\n", (void *)(uintptr_t)&main);
    printf("data (init)   g_init : %p\n", (void *)g_init);
    printf("bss  (static) s_bss  : %p\n", (void *)s_bss);
    printf("stack (local) local  : %p\n", (void *)local);
    printf("heap          heap   : %p\n", (void *)heap);

    // Show few elements (when TAILLE is small) to avoid noise
    int show = TAILLE < 5 ? TAILLE : 5;
    printf("\n--- Exemples de valeurs (premiers %d éléments) ---\n", show);
    for (int i = 0; i < show; i++) {
        printf("g_init[%d]=%d, s_bss[%d]=%d, local[%d]=%d, heap[%d]=%d\n",
               i,
               g_init[i],
               i,
               s_bss[i],
               i,
               local[i],
               i,
               heap[i]);
    }
    printf("pid du programme : %d\n", getpid());
    printf("lancer les commandes :\n");
    printf("\tcat /proc/%d/maps\n", getpid());
    printf("\tpmap %d\n", getpid());
    printf("Entrée pour terminer (lancer les commandes avant)\n");

    // attente pour inspecter /proc/<pid>/maps
    getchar();

    free(heap);
    return 0;
}
