#include <stdio.h>


char s[8]="From";

const char *getval(const char* ptr)
{

	ptr =  s;
	printf("%s\n", ptr);
	return ptr;
}

int main(int argc, char *argv[])
{
	char *ptr, *rt, *next;
	rt = getval(ptr);
	printf("main: %s\n", rt);
	char *p = "e";
	*rt = "111122222"; 
	printf("%x\n",rt);
	*next = 'r';
	printf("%x\n",next);
	printf("main: %s,%s\n", rt,s);
	
	return 0;
}
