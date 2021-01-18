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
    uint8_t buf[100];
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }

    int sock;
    socklen_t clnt_addr_sz;
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

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    {
        printf("bind() error! \n");
        return -1;
    }

    for(int i = 0; i < 5; i++)
    {
        buf[0] = NULL;

        sleep(3);
        clnt_addr_sz = sizeof(clnt_addr);
        
        int read_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);

        printf("Recv Message : %s \n", buf);
    }

    close(sock);
}