#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE	1024

char buf[BUF_SIZE];
char recev[BUF_SIZE];

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage : %s <PORT>", argv[0]);
	}

	int myFile = open("Half-Close.txt", O_RDONLY);
	if(myFile == -1)
	{
		printf("File Open error! \n");
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
		close(sock_serv);
		return -1;
	}

	if(listen(sock_serv, 5) == -1)
	{
		printf("listen() error! \n");
		close(sock_serv);
		return -1;
	}

	int clnt_addr_sz = sizeof(clnt_addr);
	sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
	if(sock_clnt == -1)
	{
		printf("accept() error! \n");
		close(sock_serv);
		return -1;
	}

	int read_len = read(myFile, buf, BUF_SIZE);
	printf("File : %s \n", buf);
	printf("Read len : %d \n", read_len);

	write(sock_clnt, buf, read_len);
	shutdown(sock_clnt, SHUT_WR);

	read(sock_clnt, recev, BUF_SIZE);
	printf("Get : %s \n", recev);

	close(sock_clnt);

	return 0;
}
