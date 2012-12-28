#include "csapp.h"

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

void handler(int sig)
{
	while (waitpid(-1, 0, WNOHANG) > 0);
	return;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	int port;
	port = atoi(argv[1]);

	Signal(SIGCHLD, handler);
	int listenfd;
	listenfd = Open_listenfd(port);

	struct sockaddr_in caddr;
	
	int fd;
	int clen = sizeof(caddr);
	while (1)
	{
		fd = Accept(listenfd, (struct sockaddr *)&caddr, &clen);
		if (Fork() == 0)
		{
			Close(listenfd);
			echo(fd);
			Close(fd);
			exit(0);
		}
	}
	Close(listenfd);
	exit(0);
}
