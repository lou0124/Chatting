#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "fd_linked_list.h"

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void connect_client(int serv_sock, int epfd, Fd_header *header);
void excute_message(int clnt_sock, int epfd, Fd_header *header);
void error_handling(char *buf);

int main(int argc, char *argv[])
{
    int serv_sock;
    struct sockaddr_in serv_adr;
    int i;
    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;
    Fd_header *header;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);    

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    header = (Fd_header *)malloc(sizeof(Fd_header));
    header->link = NULL;

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) 
        {
            puts("epoll_wait() error");
            break;
        }

        for (i=0; i<event_cnt; i++)
        {
            if (ep_events[i].data.fd == serv_sock)
                connect_client(serv_sock, epfd, header);
            else
                excute_message(ep_events[i].data.fd, epfd, header);
        }       
    }

    close(serv_sock);
    close(epfd);
    return 0;
}

void connect_client(int serv_sock, int epfd, Fd_header *header)
{
    socklen_t addr_sz;
    struct sockaddr_in clnt_adr;
    int clnt_sock;
    struct epoll_event event;

    addr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &addr_sz);
    event.events = EPOLLIN;
    event.data.fd = clnt_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
    fd_add(header, clnt_sock);
    printf("connected client: %d \n", clnt_sock); 
}

void excute_message(int clnt_sock, int epfd, Fd_header *header) 
{
    int str_len;
    char buf[BUF_SIZE];

    str_len = read(clnt_sock, buf, BUF_SIZE);
    if (str_len == 0)
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
        close(clnt_sock);
        fd_delete(header, clnt_sock);
        printf("closed client: %d \n", clnt_sock);
    }
    else
        write_fds(header, buf, str_len);
}

void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}