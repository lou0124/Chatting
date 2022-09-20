#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define NAME_SIZE 21

void error_handling(char *message);

char message[BUF_SIZE];
char name[NAME_SIZE];

void *send_msg(void *arg);
void *recv_msg(void *arg);

int main(int argc, char *argv[])
{
    int sock;
    
    int str_len;
    struct sockaddr_in serv_adr;
    pthread_t snd_thread, rev_thread;

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port= htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected..........");

    fputs("닉네임을 입력하세요.(영문 20자 이내) : ", stdout);
    fgets(name, NAME_SIZE, stdin);
    name[strlen(name)-1] = '\0';

    pthread_create(&snd_thread, NULL, send_msg, (void *)&sock);
    pthread_create(&rev_thread, NULL, recv_msg, (void *)&sock);
    pthread_join(snd_thread, NULL);
    pthread_join(rev_thread, NULL);

    close(sock);
    return 0;
} 

void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *send_msg(void *arg)
{
    int sock = *((int *)arg);
    char name_msg[NAME_SIZE + BUF_SIZE + 2];

    while(1)
    {
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "[%s] %s", name, message);
        write(sock, name_msg, strlen(name_msg));
    }

    return NULL;
}

void *recv_msg(void *arg)
{
    int sock = *((int *)arg);
    int str_len;
    char name_msg[NAME_SIZE + BUF_SIZE + 2];

    while(1)
    {
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE + 1);
        if (str_len == -1) {
            return NULL;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    
    return NULL;
}