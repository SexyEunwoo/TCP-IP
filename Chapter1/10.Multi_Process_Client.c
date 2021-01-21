#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define TRUE		1
#define FALSE		0

char ch;

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
		printf("connect() error!");
		close(sock);
		return -1;
	}

	while(TRUE)
	{
		ch = fgetc(stdin);
		write(sock, &ch, 1);
		if(ch == 'q')
			break;
	}
	
	close(sock);
	return 0;
}
