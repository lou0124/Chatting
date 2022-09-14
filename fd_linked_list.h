#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Fd_header {
    struct Fd *link;
}Fd_header;

typedef struct Fd {
    int fd;
    struct Fd *next;
}Fd;

void fd_add(Fd_header *header, int fd);
int fd_delete(Fd_header *header, int fd);
void fd_test(Fd_header *header);
void write_fds(Fd_header *header, char *buf, int str_len);