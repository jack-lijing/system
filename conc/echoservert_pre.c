#include "csapp.h"
#include "sbuf.h"

#define NTHREADS	4
#define SBUFSIZE	16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf;		/*shared buffer of connected descriptors*/

int main(int argc, char *argv[])
{
	int i, listenfd, connfd, port;
	struct sockaddr_in clientaddr;
	int clientlen = sizeof(clientaddr);
	pthread_t tid;

		if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	
	sbuf_init(&sbuf, SBUFSIZE);
	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);

	for (i =0 ; i< NTHREADS ; i++)
		Pthread_create(&tid, NULL, thread, NULL);

	while (1)
	{
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		sbuf_insert(&sbuf, connfd);
	}
}

void *thread(void *vargp)
{
	Pthread_detach(Pthread_self());
	while (1)
	{
		int connfd = sbuf_remove(&sbuf);
		echo_cnt(connfd);
		Close(connfd);
	}
}

