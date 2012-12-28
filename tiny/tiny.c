#include "csapp.h"

void doit(int);
void read_requesthdrs(rio_t *h);
int parse_uri(char* uri, char* filename, char *cgiargs);
void server_static(int fd, char *filename, int filesize);
void server_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char * shortmsg, char *longmsg);

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
		doit(conn);
		Close(conn);
	}
	exit(0);
}

void doit(int fd)
{
	rio_t rio;
	struct stat sbuf;
	Rio_readinitb(&rio, fd);
	int n;
	char method[MAXLINE],uri[MAXLINE],version[MAXLINE];
	char buf[MAXLINE], filename[MAXLINE], cgiargs[MAXLINE];
	n = Rio_readlineb(&rio, buf, MAXLINE);
	sscanf(buf, "%s %s %s", method, uri, version);
	if(strcasecmp(method, "GET") != 0)
		clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
	read_requesthdrs(&rio);

	int is_static;
	is_static = parse_uri(uri, filename, cgiargs);
	if (stat(filename, &sbuf) < 0)
	{
		clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
		return;
	}

	if(is_static)
	{
		if (!(S_ISREG(sbuf.st_mode))|| !(S_IRUSR & sbuf.st_mode))  //file is not regular or can be read
		{
			clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
			return;
		}
		server_static(fd, filename, sbuf.st_size);
	}
	else 
	{
		if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
		{
			clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't Run the CGI");
			return;
		}
		server_dynamic(fd, filename, cgiargs);
	}

}

void clienterror(int fd, char* cause, char *errnum, char *shortmsg, char *longmsg )
{
	char buf[MAXLINE], body[MAXLINE];
	
	/*build the http response body*/
	sprintf(body,"<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s:%s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

	/*Print the http response*/
	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-tye:text/html\r\n");
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-length:%d\r\n\r\n", strlen(body));
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, body, strlen(body));
	/*print*/
}

void read_requesthdrs(rio_t *rp)
{
	char buf[MAXLINE];
	Rio_readlineb(rp, buf, MAXLINE);
	while(strcmp(buf, "\r\n"))
		Rio_readlineb(rp, buf, MAXLINE);
	return;
}

int parse_uri(char* uri, char* filename, char *cgiargs)
{
	char *ptr;
	int stc;
	ptr = strstr(uri,"/cgi-bin/");
	if ( NULL == ptr)
	{
		strcpy(cgiargs, "");
		strcpy(filename, ".");
		strcat(filename, uri);
		if(uri[strlen(uri) - 1] == '/')
			strcat(filename, "home.html");
		return 1;
	}
	else 
	{
		ptr = strrchr(uri, '?');
		if (ptr)
		{
			strcpy(cgiargs, ptr+1);
			*ptr = '\0';
		}
		else
			strcpy(cgiargs, "");
		strcpy(filename, ".");
		strcat(filename, uri);
		return 0;
	}
}

void server_static(int conn, char *filename, int filesize)
{
	char filetype[MAXLINE], buf[MAXLINE];
	get_filetype(filename , filetype);
	sprintf(buf,"HTTP/1.0 200 OK\r\n");
	sprintf(buf,"%sServer:Tiny Web Server\r\n",buf);
	sprintf(buf,"%sContent-type:%s",buf, filetype);
	sprintf(buf,"%s\r\n", buf);
	Rio_writen(conn, buf, strlen(buf));

	int srcfd;
	srcfd = open(filename, O_RDONLY, 0);
	
	//function 1: use Rio_read
	rio_t rt;
	char body[MAXLINE];
	Rio_readinitb(&rt, srcfd);
	int rn,leftn = filesize;
	while((rn = Rio_readnb(&rt, body, leftn)) > 0)
	{
		Rio_writen(conn, body, leftn);
		leftn -= rn;
	}
	
	
	//function 2:: use mmap()
	/*
	char *srcp;
	srcp = mmap(0, filesize, PORT_READ, MAP_PRIVATE, srcfd, 0);
	Close(srcfd);
	Rio_writen(conn, srcp, filesize);
	Munmap(srcp,filesize);
	*/
}

void get_filetype(char *filename, char *filetype)
{
	if(strstr(filename,".html"))
		strcpy(filetype, "text/html");
	if (strstr(filename, ".gif"))
		strcpy(filetype, "image/gif");
	if(strstr(filename, ".jpg"))
		strcpy(filetype, "image/jpeg");
	else 
		strcpy(filetype, "text/plain");
}



void server_dynamic(int fd, char *filename, char *cgiargs)
{
	char buf[MAXLINE], *emptylist[] = {NULL};
	sprintf(buf,"HTTP/1.0 200 OK\r\n");
	sprintf(buf,"%sServer:Tiny Web Server\r\n",buf);
	//sprintf(buf,"%s\r\n", buf);
	Rio_writen(fd, buf, strlen(buf));


	int sum;

	if (Fork() == 0)
	{
		setenv("QUERY_STRING", cgiargs, 1);
		Dup2(fd, STDOUT_FILENO);
		execve(filename, emptylist, environ);
	}
	Wait(NULL);
}

