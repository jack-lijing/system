#include "csapp.h"

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char *argv[])
{
	int listenfd, port, *connfdp;
	int clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	pthread_t tid;

	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	
	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);

	while (1)
	{
		connfdp = Malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Pthread_create(&tid, NULL, thread, connfdp);
	}
}

void *thread(void *vargp)
{
	int connfd = *((int *) vargp);
	Pthread_detach(pthread_self());
	Free(vargp);
	echo(connfd);
	Close(connfd);
	return NULL;
}

void echo(int conn)
{
	rio_t rio;
	rio_readinitb(&rio, conn);
	char usrbuf[MAXLINE];
	int n;
	while((n = rio_readlineb(&rio, usrbuf, MAXLINE))!=0)
	{
		printf("server receive %d bytes\n",n);
		Rio_writen(conn, usrbuf, n);
	}
}

