#include "csapp.h"

int main(int argc, char *argv[])
{
	char **pp;
	struct in_addr addr;
	struct hostent *hostp;
	
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <domain name or dotted-decimal>\n", argv[0]);
		exit(0);
	}

	if(inet_aton(argv[1], &addr) != 0) //success
		hostp = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
	else
		hostp = gethostbyname(argv[1]);
	
	if(hostp == NULL )
	{
		fprintf(stderr, "Could find host: %s\n",hstrerror(h_errno));
		exit(0);
	}
	
	printf("hostname:%s\n", hostp->h_name);
	printf("hosttype:%d\n", hostp->h_addrtype);
	printf("host_length:%d\n", hostp->h_length);
	
	printf("host_aliases\n");
	while (*(hostp->h_aliases) != NULL)
	{
		printf("		%s\n", *(hostp->h_aliases));
		(hostp->h_aliases)++;
	}
	
	printf("host_addr_list\n");
	for (pp = hostp->h_addr_list; *pp != NULL ; pp++ )
	{
		addr.s_addr = *((unsigned int *)*pp);
		printf("		%s\n", inet_ntoa(addr));
	}
	exit(0);
}
