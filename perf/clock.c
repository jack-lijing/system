#include <stdio.h>
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

void access_counter(unsigned *hi, unsigned *lo)
{
	asm("rdtsc; movl %%edx, %0; movl %%eax,%1"
	:"=r" (*hi), "=r" (*lo)
	: /*No input*/
	: "%edx", "%eax");
}

void start_counter()
{
	access_counter(&cyc_hi, &cyc_lo);
}

double get_counter()
{
	unsigned ncyc_hi, ncyc_lo;
	unsigned hi, lo, borrow;
	double result;

	/*get cycle counter*/
	access_counter(&ncyc_hi, &ncyc_lo);

	/*Do double precision subtraction*/
	lo =  ncyc_lo - cyc_lo;
	borrow = lo > ncyc_lo;
	hi = ncyc_hi - cyc_hi - borrow;
	result = (double)hi *(1 << 30) * 4 + lo;
	if (result < 0)
	{
		fprintf(stderr, "Error");
	}
		return result;
}



double mhz(int verbose, int sleeptime)
{
	double rate;
	start_counter();
	sleep(sleeptime);
	rate = get_counter() / (1e6*sleeptime);
	printf("2e =  %f\n", 2e-2);
	printf("1e =  %d\n", (int)2e2);
	if (verbose)
	{
		printf("Processor clock rate ~= %.2f MHz\n", rate);
	}
	return rate;
}

int main()
{
	mhz( 1, 1);
	return;
}