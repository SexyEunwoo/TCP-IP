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
    int operand_num;
    int length = 0;
    uint8_t operator;
    uint8_t data[255];
    
    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT>", argv[0]);
        return -1;
    }

    int sock_clnt;
    struct sockaddr_in serv_addr;

    sock_clnt = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_clnt == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock_clnt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("connect() error! \n");
        return -1;
    }

    // 1. operand number
    // 2. operand 1, 2, 3, ---
    // 3. operator

    fputs("Operand num : ", stdout);
    operand_num = fgetc(stdin);
    fgetc(stdin);
    data[length++] = operand_num;

    for(int i = 0; i < operand_num; i++)
    {
        pritnf("Operand %d : ", i + 1);
        scanf("%d", &data[1 + i * 4]);
        length += i * 4;
    }

    fputs("operator : ", stdout);
    data[length++] = fgetc(stdin);

    write(sock_clnt, &data, length);
    close(sock_clnt);

    printf("Socket communicaion end!\n");
    return 0;
}