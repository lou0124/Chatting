#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Fd {
    int fd;
    struct Fd *next;
}Fd;

void fd_add(Fd *head, int fd);
int fd_delete(Fd *head, int fd);