#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	int filedes[2];
	char str[] = "Hello World!";
	char recv[BUF_SIZE];
	pipe(filedes);

	pid_t pid = fork();

	if(pid == 0)
	{
		write(filedes[1], str, sizeof(str));
	}
	else
	{
		read(filedes[0], recv, BUF_SIZE);

		printf("pipe read : %s \n", recv); 
	}

	return 0;
}
