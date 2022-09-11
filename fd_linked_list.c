#include "fd_linked_list.h"

void add(Fd *head, int fd) 
{
    Fd *temp;
    Fd *new_fd;

    new_fd = (Fd *)malloc(sizeof(Fd));
    new_fd->fd = fd;
    new_fd->next = NULL;

    if (head == NULL) {
        head = new_fd;
        return;
    }

    temp = head;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new_fd;
}

int delete(Fd *head, int fd)
{
    Fd *temp = head;
    Fd *pre = temp;

    if (head->fd == fd)
    {
        head = head->next;
        free(temp);
        return 1;
    }

    while(temp->next != NULL) 
    {
        temp = temp->next;
        if (temp->fd == fd)
        {
            pre->next = temp->next;
            free(temp);
            return 1;
        }
        pre = temp;
    }

    return 0;
}
