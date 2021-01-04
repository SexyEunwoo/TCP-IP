#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE    1
#define FALSE   0

uint8_t datas[] = {0x15, 0x03, 0x00, 0x00, 0x00, 0x02, 0x12, 0x34};
uint8_t recived[100];

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        return -1;
    }

    int sock;
    struct sockaddr_in sock_addr;
    int sock_addr_size = sizeof(sock_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("socket() Error! \n");
        return -1;
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = PF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        printf("connect() Error! \n");
        return -1;
    }

    write(sock, datas, sizeof(datas));
    
    int str_len = 0;

    while(str_len < 2)
    {
        str_len += read(sock, recived, 100);
    }
    
    printf("recived%d : %#x\n", 1, recived[0] << 8 | recived[1]);
    printf("recived%d : %#x\n", 2, recived[2] << 8 | recived[3]);
    

    close(sock);

    return 0;
}