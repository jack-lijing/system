#include "csapp.h"

int main(int argc, char **argv)
{
	struct stat dstat;
	char *type, *readok;

	stat(argv[1], &dstat);
	if(S_ISREG(dstat.st_mode))
		type = "regular";
	else if(S_ISDIR(dstat.st_mode))
		type = "directory";
	else
		type = "other";
	if((dstat.st_mode & S_IRUSR))
		readok = "yes";
	else
		readok = "no";

	printf("type: %s, read: %s\n", type, readok);
	exit(0);
}
