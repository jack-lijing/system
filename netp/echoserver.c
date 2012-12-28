#include "csapp.h"

void echo(int conn);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: %s <port>\n",argv[0]);
	}
	
	int port, listenfd;
	port = atoi(argv[1]);
	int listendfd;
	listenfd = Open_listenfd(port);
	struct sockaddr_in caddr;
	int addrlen = sizeof(caddr);
	int conn;
	while(1)
	{
		conn = Accept(listenfd, (struct sockaddr *)&caddr, (int *)&addrlen);
		printf("Client Connect: %s:%d\n", inet_ntoa(caddr.sin_addr), caddr.sin_port);
		echo(conn);
		Close(conn);
	}
	exit(0);
}

void echo(int conn)
{
	rio_t rio;
	Rio_readinitb(&rio, conn);
	char usrbuf[MAXLINE];
	int n;
	while((n = (int)Rio_readlineb(&rio, usrbuf, MAXLINE)) != 0)
	{
		printf("server receive %d bytes\n",n);
		Rio_writen(conn, usrbuf, n);
	}
} 
