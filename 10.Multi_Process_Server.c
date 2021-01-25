#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE		1
#define FALSE		0
#define MAX_CLIENT	5
#define BUF_SIZE	1024

char ch;

int main(int argc, char* argv[])
{
	int pid;
	if(argc != 2)
	{
		printf("Usage : %s <PORT> \n", argv[0]);
		return -1;
	}

	int sock_serv, sock_clnt;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_sz = sizeof(clnt_addr);
	pid_t pid_clnt[MAX_CLIENT];

	sock_serv = socket(PF_INET, SOCK_STREAM, 0);
	if(sock_serv == -1)
	{
		printf("socket() error! \n");
		close(sock_serv);
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

	for(int i = 0; i < MAX_CLIENT; i++)
	{
		sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
		if(sock_clnt == -1)
		{
			printf("accept() error \n");
			close(sock_serv);
			return 0;
		}

		pid = fork();
		if(pid == 0) // child process
		{
			break;
		}
		else // parent process
		{

		}
	}

	if(pid == 0)
	{
		while(TRUE)
		{
			read(sock_clnt, &ch, 1);
			fputc(ch, stdout);
			if(ch == 'q')
				break;
		}
	}
	else
	{
		close(sock_serv);
	}
	
	close(sock_clnt);
	close(sock_serv);
	
	return 0;
}
