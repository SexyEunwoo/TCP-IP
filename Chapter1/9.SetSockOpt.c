#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
    int sock_tcp, sock_udp;
    struct sockaddr_in sock_addr;
    int sock_addr_sz = sizeof(sock_addr);
    int sock_snd_buf, sock_opt_sz;

    sock_tcp = socket(PF_INET, SOCK_STREAM, 0);
    sock_udp = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_tcp == -1 || sock_udp == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    sock_opt_sz = sizeof(int);

    getsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, &sock_snd_buf, &sock_opt_sz);
    printf("Default send buffer : %dbyte \n", sock_snd_buf);

    sock_snd_buf = 12345;
    setsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, &sock_snd_buf, sock_opt_sz);
    printf("Changed send buffer : %dbyte \n", sock_snd_buf);

    return 0;
}