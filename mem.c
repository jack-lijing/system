#include <stdio.h>
#include "fcyc2.h"
#include "clock.h"

#define MINBYTES (1 << 10)  //FROM 1KB
#define MAXBYTES (1 << 23)  //up to 8 MB
#define MAXSTRIDE 16
#define MAXELEMS MAXBYTES / sizeof(int)

int data[MAXELEMS];

void test(int elems, int stride)
{
	int i,result = 0;
	volatile int sink;
	for(i =0; i<elems; i += stride)
		result += data[i];
	sink = result;
}

double run(int size, int stride, double Mhz)
{
	double cycles;
	int elems =size / sizeof(int);

	test(elems,stride);   				//warm up the cache
	cycles = fcyc2(test,elems,stride,0);
	return(size / stride) / (cycles / Mhz);
}

int main()
{
	int size;		// working set size(in bytes)
	int stride;
	double Mhz;

	init_data(data, MAXELEMS);
	Mhz = mhz(0); 		// estimate the clock frequency
	for (size = MAXBYTES; size >= MINBYTES; size >>= 1)
	{
		for(stride = 1; stride <= MAXSTRIDE; stride++){
			printf("%.1f\t",run(size, stride, Mhz));
		}
		printf("\n");
	}
	exit(0);
}


