#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define TRUE    1
#define FALSE   0

int main(int argc, char* argv[])
{
    char buf[100];
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return 0;
    }

    int sock, clnt_addr_sz;
    struct sockaddr_in serv_addr, clnt_addr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        return -1;
    }

    while(TRUE)
    {
        printf("Waiting... \n");
        clnt_addr_sz = sizeof(clnt_addr);
        int len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        buf[len] = '\0';
        
        if(!strcmp(buf, "q"))
        {
            break;
        }
        printf("From Client : %s \n", buf);
        // sendto(sock, buf, len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_sz);
    }

    close(sock);
    return 0;
}