#include "csapp.h"

void handler(int sig)
{
	static int beeps = 0;

	if( beeps++ <5)
	{
		printf("beef\n");	
		Alarm(1);
	}
	else
	{
		printf("boom!\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	Signal(SIGALRM, handler);

	Alarm(1);

	while(1)
	{};

	exit(0);
}
