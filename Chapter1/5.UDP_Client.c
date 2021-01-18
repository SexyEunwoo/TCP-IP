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
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        return -1;
    }

    int sock_serv;
    struct sockaddr_in serv_addr;

    sock_serv = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_serv == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    while(TRUE)
    {
        fgets(buf, sizeof(buf), stdin);
        if(!strcmp(buf, "q\n"))
        {
            sendto(sock_serv, "q", 1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
            break;
        }
        
        printf("you write %s \n", buf);
        sendto(sock_serv, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    }
    
    close(sock_serv);
    return 0;
}