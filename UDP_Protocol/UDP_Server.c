#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE        1
#define FALSE       0
#define BUF_SIZE    1024

char buf[BUF_SIZE];

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        printf("socket() error!\n");
        return -1;
    }

    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(atoi(argv[1]));

    struct sockaddr_in clientAddr;
    int clientAddr_size = sizeof(clientAddr);


    if(bind(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
    {
        printf("bind() error! \n");
        return -1;
    }

    while(TRUE)
    {
        int str_len = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddr_size);
        sendto(sock, buf, str_len, 0, (struct sockaddr*)&clientAddr, clientAddr_size);
    }
    close(sock);

    return 0;
}