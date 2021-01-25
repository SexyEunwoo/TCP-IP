#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE	1024

char buf[BUF_SIZE];

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
		close(sock);
		return -1;
	}

	int len = 0;
	
	while(len = read(sock, buf, BUF_SIZE) != 0);

	printf("Received Data : %s \n", buf);
	write(sock, "Thank you!", 10);

	close(sock);
	return 0;
}
