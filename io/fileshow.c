#include "csapp.h"

int main(int argc, char *argv[])
{
	int rn, fd;
	char bufarray[1024];
	char *buf = bufarray;
	rio_t rio;
	fd = open(argv[1], O_RDONLY, NULL);
	if(fd < 0)
	{
		printf("open error\n");
		exit(0);
	}

	rio_readinitb(&rio, fd);
	rn = rio_readnb(&rio, buf, 1024);
	printf("%x\n bytes:%d\n", buf, rn);
	
	return 0;
}
