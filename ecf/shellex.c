#include "csapp.h"

#define MAXARGS	128
#define MAXLINE	12

extern char **environ;

void eval(char * cmdline);
int parseline(const char *cmdline, char **argv);
int builtin_command(char **argv);

int main(int argc, char *argv[])
{
	char cmdline[MAXLINE];

	while(1){
		/*read*/
		printf("> ");
		Fgets(cmdline, MAXLINE, stdin);
		if(feof(stdin))exit(0);
			/*evaluate*/
		eval(cmdline);
	}


}

void eval(char *cmdline)
{
	char *argv[MAXARGS];		/*argv for execve()*/
	int bg;						/*should be job run in bg or fg*/
	pid_t pid;					/*process id */

	bg = parseline(cmdline, argv);
	if (argv[0] == NULL)
		return;
	if(!builtin_command(argv)){
		if ((pid = Fork()) == 0)
		{
			if (execve(argv[0], argv, environ) < 0)
			{
				printf( "%s: Command not found \n", argv[0]);
				exit(0);
			}
			else
				printf( "%s: run \n", argv[0]);
		}
		/*parent wait for foreground job to terminate*/
		if (!bg)
		{
			int status;
			if (waitpid(pid, &status, 0) < 0)
				unix_error("waitfg: waitpid error");
		}
		else
			printf("%d %s", pid, cmdline);
	}
	return;
}

/*if the first arg is buildin command ,run it and return true*/
int builtin_command(char **argv)
{
	if ( !strcmp(argv[0], "quit"))
	{
		printf(" will be run quit command \n");
		exit(0);
	}
	if ( !strcmp(argv[0], "&"))
	{
		return 1;			// if the first argv is "&", exit(0)
	}
	return 0;
}

/*parse the command line the build the argv array  
	return 0: fg
	return 1: bg
*/
int parseline(const char *cmdline, char **argv)
{
	char *delim;			/*points to first space delimiter*/
	char argc = 0 ;			/*argv number*/
	//int bg;				/*return value, bg(&) or not*/
	char *buf = cmdline;

	buf[strlen(buf) - 1] = ' ';  // replace '\n' withe ' ', so the ' ' is the unique split tag in the command

	while( *buf && (*buf == " "))			//ignore the ' ' at first
		buf++;
	
	while( (delim = strchr( buf, ' ')) )
	{
		*delim = '\0';
		argv[argc++] = buf;
		buf = delim + 1;
		while( *buf && *buf == " ")			//ignore the ' ' at first
			buf++;
	}
	argv[argc] = NULL;

	if ( argc == 0)		// ignore blank line
	{
		return 1;
	}

	if( *argv[argc - 1] == '&' )
	{
		argv[argc--] = NULL;		// delete the last args '&' and return 1;
		return 1;
	}
	else 
		return 0;
}