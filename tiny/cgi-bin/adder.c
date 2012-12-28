#include "csapp.h"

int main(int argc, char *argv[])
{
	char *param, *ptr, content[MAXLINE];
	int a, b, sum;
	param = getenv("QUERY_STRING");
	if(NULL != param)
	{
		char argv1[MAXLINE],argv2[MAXLINE], content[MAXLINE];
		ptr = strchr(param,'&');
		*ptr = '\0';

		strcpy(argv1, param);
		strcpy(argv2, ptr+1);
		a = atoi(argv1);
		b = atoi(argv2);
		sum = a + b;
	}
	sprintf(content, "Welcome to add.com \r\n");
	sprintf(content,"%s The answer is : %d + %d = %d \r\n", content, a, b, sum);
	
	printf("Content-length: %d \r\n", strlen(content));
	printf("Content-type: text\html\r\n\r\n");
	printf("%s", content);

	fflush(stdout);
	exit(0);
}
