#include "csapp.h"

void echo(int connfd);
void command(void);


int main(int argc, char *argv[])
{
	int listenfd, connfd, port;
	int clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	fd_set read_set, ready_set;

	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	
	port = atoi(argv[1]);

	
	listenfd = Open_listenfd(port);

	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO, &read_set);
	FD_SET(listenfd, &read_set);

	while(1)
	{
		ready_set = read_set;
		Select(listenfd+1, &ready_set, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &ready_set))
		{
			command();
		}
		if (FD_ISSET(listenfd, &ready_set))
		{
			connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
			echo(connfd);
			Close(connfd);
		}
	}

	Close(listenfd);
	return 0;
}

void echo(int conn)
{
	rio_t rio;
	Rio_readinitb(&rio, conn);
	char usrbuf[MAXLINE];
	int n;
	while((n = Rio_readlineb(&rio, usrbuf, MAXLINE))!=0)
	{
		printf("server receive %d bytes\n",n);
		Rio_writen(conn, usrbuf, n);
	}
}

void command(void)
{
	char buf[MAXLINE];
	if (!Fgets(buf, MAXLINE, stdin))
	{
		exit(0);
	}
	printf("%s", buf);
}
