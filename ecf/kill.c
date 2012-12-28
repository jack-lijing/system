#include "csapp.h"


int main()
{
	pid_t pid;

	pid = Fork();

	if (pid == 0)
	{
		Pause();
		printf("pause\n");
		exit(0);
	}

	kill(pid, SIGKILL);
	exit(0);
}
