#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE        1
#define FALSE       0
#define BUF_SIZE    1024
char buf[BUF_SIZE];
char sendBuf[BUF_SIZE];
/*
1. 파일이름을 전송받는다 ( 파일 이름의 구분은 \n으로 구분한다 )
2. 파일이 존재할 경우 파일을 전송하고 파일이 존재하지 않을 경우 그냥 연결종료
*/

void socket_function(int sock_client);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }

    int sock_server;
    struct sockaddr_in serv_addr;

    int sock_client;
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    
    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_server == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        return -1;
    }

    if(listen(sock_server, 5))
    {
        printf("listen() error! \n");
        return -1;
    }

    sock_client = accept(sock_server, (struct sockaddr*)&client_addr, &client_addr_size);
    if(sock_client == -1)
    {
        printf("accept() error! \n");
        return -1;
    }

    printf("Connected! \n");
    socket_function(sock_client);
    close(sock_client);
    close(sock_server);

    return 0;
}

void socket_function(int sock_client)
{
    int ret = FALSE;
    int str_len = 0;

    while(TRUE)
    {
        str_len += read(sock_client, buf + str_len, 1);

        if(buf[str_len - 1] == '\n')
            break;
    }
    buf[str_len-1] = '\0';

    FILE* f = fopen(buf, "rb");
    if(f == NULL)
    {
        printf("fopen() error!\n");
        return ;
    }

    int read_len = fread(sendBuf, 1, BUF_SIZE, f);
    write(sock_client, sendBuf, read_len);
    write(sock_client, "\0", 1);

}