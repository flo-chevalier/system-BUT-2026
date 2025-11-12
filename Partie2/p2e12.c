/*
 p2e12.c - Éviter les accès concurrents (flock)
 Ouvre p2e12.txt, lit un entier, l'incrémente et réécrit la valeur en protégé
 par flock.
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

int main(void) {
    const char *fname = "p2e12.txt";
    int fd = open(fname, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (flock(fd, LOCK_EX) != 0) {
        perror("flock");
        close(fd);
        return 1;
    }

    /* lire entier présent (si vide, considérer 0) */
    off_t off = lseek(fd, 0, SEEK_SET);
    if (off == (off_t)-1) {
        perror("lseek");
        flock(fd, LOCK_UN);
        close(fd);
        return 1;
    }

    char buf[64] = {0};
    ssize_t r = read(fd, buf, sizeof(buf) - 1);
    long val = 0;
    if (r > 0) {
        val = strtol(buf, NULL, 10);
    }

    val += 1;

    /* réécrire : tronquer puis écrire */
    if (ftruncate(fd, 0) != 0) {
        perror("ftruncate");
        flock(fd, LOCK_UN);
        close(fd);
        return 1;
    }
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        perror("lseek2");
        flock(fd, LOCK_UN);
        close(fd);
        return 1;
    }

    char out[64];
    int n = snprintf(out, sizeof(out), "%ld\n", val);
    if (write(fd, out, (size_t)n) != n) {
        perror("write");
        flock(fd, LOCK_UN);
        close(fd);
        return 1;
    }

    /* déverrouiller et fermer */
    if (flock(fd, LOCK_UN) != 0) {
        perror("flock unlock");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
