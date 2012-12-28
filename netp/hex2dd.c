#include "csapp.h"

int main(int argc, char *argv[])
{
	if( argc != 2)
	{
		printf("usage: hex2dd 0xffffffff\n");
		exit(0);
	}
	if( strncmp( argv[1], "0x", 2) != 0 || strlen(argv[1]) > 10)
	{
		printf("usage: hex2dd 0xffffffff\n");
		exit(0);
	}
	
	char *ptr;
	ptr = argv[1] + 2;   //omit "0x"
	int i, sum = 0, bit = 0;
	int tag = 0;
	for(i = 0; i < 8; i++)
	{
		if(((*ptr >= '0') && (*ptr <= '9')) )
			bit = *ptr - '0';
		else if (((*ptr >= 'a') && (*ptr <= 'f')))
			bit = *ptr - 'a' + 10;
		else 
		{
			printf("usage: hex2dd 0xffffffff\n");
			exit(0);
		}

		if( tag == 0)
		{
			sum = bit * 16;
			tag = 1;
		}
		else
		{
			sum +=  bit;
			
			
			if( i < 7)
				printf("%d.", sum);
			else
				printf("%d \n", sum);

			sum = 0;
			tag = 0;
			bit = 0;
		}
		ptr++;
	}

	return 0;
}
