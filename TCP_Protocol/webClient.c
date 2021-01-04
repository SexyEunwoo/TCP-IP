#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE        1
#define FALSE       0

uint8_t ch;

int main(void)
{
    struct hostent *host_entry;
    host_entry = gethostbyname( "www.google.com");

    if (!host_entry)
    {
        printf( "gethostbyname() 실행 실패/n");
        exit( 1);
    }
    
    int sock; 
    struct sockaddr_in sock_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("socket() error! \n");
        return -1;
    }
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[0]));
    sock_addr.sin_port = htons(80);

    
    fputs("connect before", stdout);
    fputc('\n', stdout);

    if(connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        printf("connect() error! \n");
        close(sock);
        return -1;
    }
    fputs("connect after", stdout);

    fputs("write before", stdout);
    write(sock, "GET / HTTP/1.1\r\nHost: www.google.co.kr\r\n\r\n", strlen("GET / HTTP/1.1\r\nHost: www.google.co.kr\r\n\r\n"));
    fputs("write after", stdout);

    
    while(TRUE)
    {
        read(sock, &ch, 1);
        fputc(ch, stdout);
    }
    
    return 0;
}