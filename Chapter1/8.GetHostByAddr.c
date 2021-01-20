#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("125.209.222.142");
    struct hostent* host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

    printf("domain name : %s \n\n", host->h_name);
    for(int i = 0; host->h_aliases[i]; i++)
    {
        printf("domain %d alias : %s \n", i + 1, host->h_aliases[i]);
    }

    printf("\ndomain h_addr_type : %d \n\n", host->h_addrtype);
    printf("domain h_length : %d \n\n", host->h_length);

    for(int i = 0; host->h_addr_list[i]; i++)
    {
        printf("domain %d h_addr_list : %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}