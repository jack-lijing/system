#include <stdio.h>

#define MAX 1024
int main(int argc, char *argv[])
{
	char msg[MAX] = "Hello World,haha";
	char buf1[MAX], buf2[MAX];
	sscanf(msg, "%s d[,]h %s", buf1, buf2);
	printf("buf1:%s\n buf2:%s \n", buf1, buf2);

	return 0;
}
