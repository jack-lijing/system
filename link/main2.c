#include <stdio.h>

void addvec(int *, int *, int *, int);
void mult(int *, int *, int *, int);

int x[2] = {1,2};
int y[2] = {3,4};
int z[2];


int main(int argc, char *argv[])
{
	addvec(x, y, z, 2);
	printf( "z =[%d %d]\n", z[0], z[1]);
	
	return 0;
}
