#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char buf[] = "This is test message";
    if(argc != 2)
    {
        printf("Usage : %s <PORT>", argv[0]);
        return -1;
    }

    int sock_serv, sock_clnt;
    struct sockaddr_in serv_addr, clnt_addr;

    sock_serv = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_serv == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        return -1;
    }

    if(listen(sock_serv, 5) == -1)
    {
        printf("listen() error! \n");
        return -1;
    }

    int clnt_addr_size = sizeof(clnt_addr);
    sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(sock_clnt == -1)
    {
        printf("accept() error! \n");
        return -1;
    }

    write(sock_clnt, buf, sizeof(buf));
    close(sock_clnt);
    close(sock_serv);

    printf("Socket communicaion end!\n");
    return 0;
}