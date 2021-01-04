#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define FILE_SIZE   100
#define TRUE        1
#define FALSE       0
/*
1. 파일이름을 입력받는다.
2. 입력받은 파일 이름을 서버에게 전송한다.
3. 받은 파일을 저장한다.
*/

char fileName[FILE_SIZE];
char readBuf[FILE_SIZE];

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        return -1;
    }

    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("socket() error! \n");
        return -1;
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("connect() error! \n");
        return -1;
    }

    fgets(fileName, FILE_SIZE, stdin);

    write(sock, fileName, strlen(fileName));

    int read_len = 0;

    
    while(TRUE)
    {
        read_len += read(sock, readBuf + read_len, 1);

        if(readBuf[read_len -1] == '\0')
            break;
    }

    fputs(readBuf, stdout);
    
    close(sock);

    return 0;
}