#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define TRUE        1
#define FALSE       0
#define BUF_SIZE    1024
#define MAX_CLIENT  10

pthread_t tid[MAX_CLIENT];
pthread_mutex_t mutex_lock;
int clients[MAX_CLIENT];
uint8_t isUsed[MAX_CLIENT];
uint8_t cnt = 0;
uint8_t idx = 0;


enum FUNCTION_CODE
{
    READ_COIL =             0x01,
    READ_DISCRETE_INPUT =   0x02,
    READ_MULTI_HREG =       0x03,
    READ_INPUT_REG  =       0x04,
    WRITE_SINGLE_COIL =     0x05,
    WRITE_SINGLE_HREG =     0x06,
    READ_EXCEPTION_STATUS = 0x07
};

void *threadFunc(void* argv);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        return -1;
    }

    pthread_mutex_init(&mutex_lock, NULL);

    int socket_server;
    struct sockaddr_in serv_addr;

    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_server == -1)
    {
        printf("socket() error! \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    printf("PORT : %d \n", serv_addr.sin_port);

    if(bind(socket_server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("bind() error! \n");
        return -1;
    }

    if(listen(socket_server, 10) == -1)
    {
        printf("listen() error! \n");
        return -1;
    }

    while(TRUE)
    {
        idx = -1;
        for(int i = 0; i < MAX_CLIENT; i++)
        {
            isUsed[i] == FALSE;
            idx = i;
            break;
        }
        if(idx != -1)
        {
            clients[idx] = accept(socket_server, (struct sockaddr*)&client_addr, &client_addr_size);
            printf("Connected with %d \n", idx+1);
            pthread_create(&tid[idx], NULL, threadFunc, (void*)&clients[idx]);
            isUsed[idx] = TRUE;
            cnt++;
        }
        else
        {
            while(cnt >= MAX_CLIENT);
        }

    }



    return 0;
}

void *threadFunc(void* arg)
{
    int* sock = (int*)arg;  // client socket번호
    uint8_t str_len = 0;
    char recv[BUF_SIZE];

    while(TRUE)
    {
        while(str_len < 2)
        {
            str_len += read(*sock, recv + str_len, BUF_SIZE);
            printf("str_len : %d \n", str_len);
        }
        switch(recv[1])
        {
            case 0x03:  // read Hreg
                printf("Address : %#x\n", recv[0]);
                printf("Function code : %#x\n", recv[1]);
                while(str_len < 8) str_len += read(*sock, recv + str_len, BUF_SIZE);
                printf("Start Addr : %#x \n", (recv[2] << 8 | recv[3]));
                printf("Length : %#x \n", (recv[4] << 8 | recv[5]));
                printf("CRC : %#x \n", (recv[6] << 8 | recv[7]));
                break;
            default:
                str_len = 0;
                break;
        }
        uint8_t sendBuf[] = {0x00, 0x23, 0xF0, 0x5A};
        write(*sock, sendBuf, sizeof(sendBuf));
        break;
    }    
    close(*sock);
}