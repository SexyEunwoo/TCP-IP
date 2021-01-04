#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE        1
#define FALSE       0
#define BUF_SIZE    1024

char buf[BUF_SIZE];
char ch;

void error_handling(char * message);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        return -1;
    }

    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error!");
    }

    while(TRUE)
    {
        ch = fgetc(stdin);
        write(sock, &ch, 1);
        if(ch == 'q' || ch == 'Q')
            break;
    }

    printf("Disconnected! \n");
    close(sock);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}