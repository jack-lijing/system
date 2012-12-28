#include "csapp.h"

typedef struct{
	int maxfd;			/*largest descriptor in read_set*/
	fd_set read_set;	/*set of all active descriptors*/
	fd_set ready_set;	/*subset of descriptors ready for reading*/
	int nready;			/*number of ready descriptor from select*/
	int maxi;			/*highwater index into client array*/
	int clientfd[FD_SETSIZE];	/*set of active descriptors*/
	rio_t client[FD_SETSIZE];	/*set of active read buffers*/
}pool;

int byte_cnt = 0; //counts total bytes received by server

void init_pool(int listenfd, pool *p)
{
	int i;
	p->maxi = -1;
	for (i = 0; i < FD_SETSIZE ; i++ )
	{
		p->clientfd[i] = -1;
	}

	p->maxfd = listenfd;
	FD_ZERO(&p->read_set);
	FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p)
{
	int i;
	p->nready--;
	for ( i = 0; i < FD_SETSIZE ; i++) /*find an available slot*/
		if (p->clientfd[i] < 0)
		{
			/*add connected descriptor to the pool*/
			p->clientfd[i] = connfd;
			Rio_readinitb(&p->client[i], connfd);
			/*Add the desc to desc set*/
			FD_SET(connfd, &p->read_set);

			/*Upload max desc and pool highwater mark*/
			if(connfd > p->maxfd)
				p->maxfd = connfd;
			if(i > p->maxi)
				p->maxi = i;
			break;
		}
	if(i == FD_SETSIZE) /*Couldn't find an empty slot*/
		app_error("add_client error: to many clients");
}


void check_clients(pool *p)
{
	int i, connfd, n;
	char buf[MAXLINE];
	rio_t rio;

	for (i = 0; (i <= p->maxi) && (p->nready >0 ); i++ )
	{
		connfd = p->clientfd[i];
		rio = p->client[i];

		if ((connfd > 0)&&(FD_ISSET(connfd, &p->ready_set)))
		{
			p->nready--;
			if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
			{
				byte_cnt += n;
				printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
				Rio_writen(connfd, buf, n);
			}
			else
			{
				Close(connfd);
				FD_CLR(connfd, &p->read_set);
				p->clientfd[i] = -1;
			}
		}
	}
}


int main(int argc, char *argv[])
{
	struct sockaddr_in clientaddr;
	int listenfd, connfd, port, clientlen = sizeof(clientaddr);

	static pool pool;
	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	
	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);
	init_pool(listenfd, &pool);
	
	while (1)
	{
		/*wait for listening/connected descriptor to become ready*/
		pool.ready_set = pool.read_set;
		pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

		/*if listening descriptor ready, add new client to pool*/
		if (FD_ISSET(listenfd, &pool.ready_set))
		{
			connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
			add_client(connfd, &pool);
		}

		/**Echo a text line from echa ready connected decriptor*/
		check_clients(&pool);
	}
}
