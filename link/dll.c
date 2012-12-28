#include <stdio.h>
#include <dlfcn.h>

int x[2] = {1,2};
int y[2] = {3,4};
int z[2];

int main(int argc, char *argv[])
{
	void *handle;
	void (*addvec)(int *, int *, int *, int);
	char *error;

	/*dynamically load the shared lib that contain addvec*/
	handle = dlopen( "/root/link/libvector.so", RTLD_LAZY);
	if( !handle)
	{
		fprintf( stderr, " cann't open %s\n", dlerror());
		exit(1);
	}

	/* get a pointer to the addvec() func we just loaded */
	addvec = dlsym( handle, "addvec");

	if((error = dlerror()) != NULL)
	{
		fprintf( stderr,"can't call %s\n", error);
		exit(1);
	}

	/*Now we can call addvec() */
	addvec(x, y, z ,2);
	printf( "z =[%d %d]\n", z[0], z[1]);

	if(dlclose(handle) == 1 )
	{
		fprintf( stderr,"can't close %s\n", error);
		exit(1);
	}
	return 0;
}

