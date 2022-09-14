#include "fd_linked_list.h"

void fd_add(Fd_header *header, int fd) 
{
    Fd *temp;
    Fd *new_fd;

    new_fd = (Fd *)malloc(sizeof(Fd));
    new_fd->fd = fd;
    new_fd->next = NULL;

    if (header->link == NULL) {
        header->link = new_fd;
        return;
    }

    temp = header->link;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new_fd;
}

int fd_delete(Fd_header *header, int fd)
{
    Fd *temp = header->link;
    Fd *pre = temp;

    if (temp->fd == fd)
    {
        header->link = temp->next;
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

void fd_test(Fd_header *header)
{
    Fd *temp = header->link; 
    while(temp != NULL)
    {
        printf("%d ", temp->fd);
        temp = temp->next;
    }
    puts("\n");    
}

void write_fds(Fd_header *header, char *buf, int str_len)
{
    Fd *temp = header->link; 
    while(temp != NULL)
    {
        write(temp->fd, buf, str_len);
        temp = temp->next;
    }
}
