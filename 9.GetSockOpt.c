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
    
    int sock_tcp_type, sock_udp_type;
    int sock_send_buf, sock_recv_buf;
    socklen_t sock_type_sz;

    sock_tcp = socket(PF_INET, SOCK_STREAM, 0);
    sock_udp = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_tcp == -1 || sock_udp == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    sock_type_sz = sizeof(socklen_t);
    getsockopt(sock_tcp, SOL_SOCKET, SO_TYPE, &sock_tcp_type, &sock_type_sz);
    getsockopt(sock_udp, SOL_SOCKET, SO_TYPE, &sock_udp_type, &sock_type_sz);
    getsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, &sock_send_buf, &sock_type_sz);
    getsockopt(sock_tcp, SOL_SOCKET, SO_RCVBUF, &sock_recv_buf, &sock_type_sz);

    printf("tcp socket SO_TYPE : %d \n", sock_tcp_type);
    printf("udp socket SO_TYPE : %d \n", sock_udp_type);
    printf("tcp socket SO_TYPE : %d \n", sock_send_buf);
    printf("tcp socket SO_TYPE : %d \n", sock_recv_buf);

    return 0;
}