#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define TRUE    1
#define FALSE   0

int calculator(uint8_t operator, uint8_t* data, int num)
{
    int ret = 0;
    switch(operator)
    {
        case '+':
            for(int i = 0; i < num; i++) ret += (int)data[i * 4];
            break;
        case '-':
            for(int i = 0; i < num; i++) ret -= (int)data[i * 4];
            break;
        case '*':
            ret = 1;
            for(int i = 0; i < num; i++) ret *= (int)data[i * 4];
            break;
        default:
            ret = -1;
    }

    return ret;
}

int main(int argc, char* argv[])
{
    uint8_t datas[100];
    uint8_t operand_num, operator;
    int length = 0;

    if(argc != 2)
    {
        printf("Usage : %s <PORT>", argv[0]);
        return -1;
    }

    int sock_serv, sock_clnt;
    struct sockaddr_in serv_addr, clnt_addr;

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
        return -1;
    }

    if(listen(sock_serv, 5) == -1)
    {
        printf("listen() error! \n");
        return -1;
    }

    int clnt_addr_size = sizeof(clnt_addr);
    sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(sock_clnt == -1)
    {
        printf("accept() error! \n");
        return -1;
    }

    read(sock_clnt, &operand_num, 1);
    printf("Get Operand Num : %d \n", operand_num - 48);

    while(TRUE)
    {
        length += read(sock_clnt, datas, (operand_num - 48) * 4);
        
        if(length >= (operand_num - 48) * 4)
            break;
    }
    
    read(sock_clnt, &operator, 1);

    int ret = calculator(operator, datas, operand_num - 48);
    printf("Calc : %d \n", ret);
    write(sock_clnt, &ret, 4);

    close(sock_clnt); close(sock_serv);

    printf("Socket communicaion end!\n");
    return 0;
}