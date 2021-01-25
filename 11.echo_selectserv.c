#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE    1024
#define TRUE        1
#define FALSE       0

char buf[BUF_SIZE];

int main(int argc, char* argv[])
{
    fd_set reads, temps;
    int ret, str_len;
    int fd_max = 0;
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }

    int sock_serv, sock_clnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    sock_serv = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_serv == -1)
    {
        printf("socket() error! \n");
        return -1;
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        close(sock_serv);
        return -1;
    }

    if(listen(sock_serv, 5) == -1)
    {
        printf("listen() error! \n");
        close(sock_serv);
        return -1;
    }

    FD_ZERO(&reads);
    FD_SET(sock_serv, &reads);
    fd_max = sock_serv;
    while(TRUE)
    {
        printf("fd_max : %d\n", fd_max);
        temps = reads;
        ret = select(fd_max + 1, &temps, 0, 0, &timeout);
        if(ret == -1)
        {
            printf("select() error! \n");
            break;
        }
        else if(ret == 0)
        {
            puts("time out!");
            continue;
        }
        else
        {
            for(int i = 0; i < (fd_max + 1); i++)
            {
                if(FD_ISSET(i, &temps))
                {
                    if(i == sock_serv)
                    {
                        sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
                        if(sock_clnt == -1)
                        {
                            printf("accept() error! \n");
                        }
                        else
                        {
                            FD_SET(sock_clnt, &reads);
                            fd_max = sock_clnt;
                            printf("Conencted with %d \n", sock_clnt);
                        }
                    }
                    else
                    {
                        str_len = read(i, buf, BUF_SIZE);
                        if(str_len == 0)
                        {
                            FD_CLR(i, &reads);
                            fd_max--;
                            close(i);
                            printf("Closed Client : %d \n", i);
                        }
                        else
                        {
                            buf[str_len] = '\0';
                            printf("Get Message : %s \n", buf);
                        }
                    }
                }
            }
        }
    }
    close(sock_serv);
}