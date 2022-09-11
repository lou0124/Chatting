#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Fd {
    int fd;
    struct Fd *next;
}Fd;

void add(Fd *head, int fd);
int delete(Fd *head, int fd);