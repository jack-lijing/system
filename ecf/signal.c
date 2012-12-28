#include "csapp.h"

#define MAXBUF	1024

void handler(int sig)
{
	pid_t pid;

	while ((pid = waitpid(-1, NULL, 0)) > 0)
	{
		printf("Handler reaped child %d \n", (int)pid);
	}

	if( errno != ECHILD )
	{
		unix_error(" waitpid error");
	}

	printf("Handler call is exit\n");
	Sleep(2);
	return;
}


int main(int argc, char *argv[])
{
	pid_t pid;
	char buf[MAXBUF];
	if(signal(SIGCHLD, handler) == SIG_ERR)
		unix_error("singal error");
	int i;
	/*parent creates children*/
	for( i=0; i<3; i++)
	{
		if((pid = Fork()) == 0)
		{
			printf("hello from children %d\n", (int)getpid());
			Sleep(1);
			exit(0);
		}

	}
	int n;
	if((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0 )
	{
		unix_error("read error");
	}
	printf("Input from parent\n");
	while(1)
	{
		;
	}
	exit(0);
}
