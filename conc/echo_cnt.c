#include "csapp.h"

static int byte_cnt;	/*byte counter*/
static sem_t mutex;

static void init_echo_cnt(void)
{
	Sem_init(&mutex, 0, 1);
	byte_cnt = 0;
}

void echo_cnt(int connfd)
{
	int n;
	char buf[MAXLINE];
	rio_t rio;
	static pthread_once_t once = PTHREAD_ONCE_INIT;

	Pthread_once(&once, init_echo_cnt);
	Rio_readinitb(&rio, connfd);
	while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	{
		P(&mutex);
		byte_cnt += n;
		printf("thread %d received %d (%d total) bytes on fd %d\n", (int)pthread_self(), n, byte_cnt, connfd);
		V(&mutex);
		Rio_writen(connfd, buf, n);
	}
}

