#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    1024
#define OPSZ        4
#define TRUE        1
#define FALSE       0

char buf[BUF_SIZE];
uint8_t ch;
uint32_t n = 0;

void error_handling(char* message);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n");
        return -1;
    }

    int sock_server;
    struct sockaddr_in serv_addr;
    
    int sock_client;
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    char opinfo[BUF_SIZE];
    int result, opnd_cnt, i;
    int recv_cnt, recv_len;

    sock_server = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_server == -1)
    {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error!");
    }

    if(listen(sock_server, 5) == -1)
    {
        error_handling("listen() error!");
    }
    sock_client = accept(sock_server, (struct sockaddr*)&client_addr, &client_addr_size);

    while(TRUE)
    {
        
        read(sock_client, &ch, 1);
        if(ch == 'q' || ch == 'Q')
            break;
        
        printf("%d\n", ch);
    }
    close(sock_server);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}