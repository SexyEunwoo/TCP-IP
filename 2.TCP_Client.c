#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char buf[] = "This is test message";
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT>", argv[0]);
        return -1;
    }

    int sock_clnt;
    struct sockaddr_in serv_addr;

    sock_clnt = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_clnt == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock_clnt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("connect() error! \n");
        return -1;
    }

    int str_len = read(sock_clnt, buf, sizeof(buf) - 1);
    if(str_len == -1)
    {
        printf("read() error! \n");
        close(sock_clnt);
        return -1;
    }

    printf("Get Message : %s \n", buf);
    
    close(sock_clnt);
    printf("Socket communication end! \n");
    
    return 0;
}