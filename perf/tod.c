#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

static struct timeval tstart;


/**Record current*/
void start_timer()
{
	gettimeofday(&tstart, NULL);
}

/*Get number of seconds since last call to start_timer*/
double get_timer()
{
	struct timeval tfinish;
	long sec, usec;
	
	gettimeofday(&tfinish, NULL);
	sec = tfinish.tv_sec - tstart.tv_sec;
	usec = tfinish.tv_usec - tstart.tv_usec;
	return sec + 1e-6*usec;
}

static double sum = 0;

void handler(int sig)
{
	printf("second = %8f\n", sum);
	exit(0);
}

int main()
{
	int i = 1000000;
	 
	signal(SIGALRM, handler);

	

//	alarm(10);
	while(i)
	{
		start_timer();
		sum += get_timer();
		i--;
	}
	printf("second = %8f\n", sum);
}