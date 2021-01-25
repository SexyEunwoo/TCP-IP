#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status;
	pid_t pid = fork();

	if(pid == 0)
	{
		sleep(13);
		exit(12);
	}
	else
	{
		while(!waitpid(-1, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec");
		}

		if(WIFEXITED(status))
		{
			printf("Normal termination \n");
			printf("Exit code : %d \n", WEXITSTATUS(status));
		}
	}
	return 0;
}
