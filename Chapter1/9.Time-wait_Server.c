#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE    1024

int main(int argc, char* argv[])
{
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }
    int sock, sock_clnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    int so_reuseaddr = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(int));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        close(sock);
        return -1;
    }
    
    if(listen(sock, 5) == -1)
    {
        printf("listen() error! \n");
        close(sock);
        return -1;
    }

    sock_clnt = accept(sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);

    int strlen;
    while((strlen = read(sock_clnt, buf, BUF_SIZE)) != 0)
    {
        fputs(buf, stdout);
    }

    return 0;
}