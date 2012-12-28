#include "csapp.h"

#define MAXLEN	1024

//static set<int> jobset[MAXLEN]

void initialjob()
{

}

void addjob(int pid)
{
	printf("add %d \n", pid);
}

void deletejob(int pid)
{
	printf("delete %d \n", pid);
}


void handler(int signum)
{
	pid_t pid;
	if((pid = waitpid(-1, NULL, 0)) > 0 )
		deletejob(pid);
	if(errno == ECHILD)
		unix_error("waitpid no child process\n");
	Sleep(2);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	sigset_t mask;
	int i;

	Signal(SIGCHLD, handler);

	while(1)
	{
		Sigemptyset(&mask);
		Sigaddset(&mask, SIGCHLD);
		Sigprocmask(SIG_BLOCK, &mask, NULL);

		if((pid = Fork()) == 0)
		{
			Sigprocmask(SIG_UNBLOCK, &mask, NULL);
			execve("/bin/ls",argv, NULL);
		}

		addjob(pid);
		Sigprocmask(SIG_UNBLOCK, &mask, NULL);
		
	}
	exit(0);
}
