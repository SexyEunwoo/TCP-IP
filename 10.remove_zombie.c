#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

void siga(int sig)
{
	if(sig == SIGCHLD)
	{
		printf("Child termination \n");
	}
}

int main()
{
	struct sigaction act;
	act.sa_handler = siga;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(SIGCHLD, &act, 0);

	pid_t pid = fork();
	
	if(pid == 0)
	{
		sleep(8);
		return 123;
	}
	else
	{
		printf("Waiting 20s \n");
		sleep(20);
	}

	return 0;
}
