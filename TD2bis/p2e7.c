/*
 p2e7.c - Lecture partielle (seek POSIX)
 Lit 10 octets après les 15 premiers octets.
 Usage: p2e7 FILE
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return 2;
    }
    const char *path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    off_t res = lseek(fd, 15, SEEK_SET);
    if (res == (off_t)-1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    char buf[11] = {0};
    ssize_t r = read(fd, buf, 10);
    if (r < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    /* afficher exactement les octets lus */
    if (write(1, buf, (size_t)r) < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    write(1, "\n", 1);
    close(fd);
    return 0;
}
