#include "/root/ecf/csapp.h"

int main(int argc, char *argv[])
{
	char c;

	while(read(STDIN_FILENO,&c, 0) != 0)
		write(STDOUT_FILENO, &c, 0);
	//printf("Hello, world\n");
	
	return 0;
}
