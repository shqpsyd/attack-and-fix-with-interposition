#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <dlfcn.h>
#include"support.h"

/* load_and_invoke() - load the given .so and execute the specified function */
void load_and_invoke(char *libname, char *funcname)
{
	/* TODO: complete this function */
	
	void * handle = dlopen(libname,RTLD_NOW|RTLD_GLOBAL);
	if(!handle){
		fputs (dlerror(), stderr);
		exit(1);
	}
	void (*func)() = dlsym(handle,funcname);
	if (!func) {
		fputs (dlerror(), stderr);
		exit(1);
	}
	func();
	dlclose(handle);
}

/* help() - Print a help message. */
void help(char *progname)
{
	printf("Usage: %s [OPTIONS]\n", progname);
	printf("Load the given .so and run the requested function from that .so\n");
	printf("  -l [string] The name of the .so to load\n");
	printf("  -f [string] The name of the function within that .so to run\n");
}

/* main() - The main routine parses arguments and invokes hello */
int main(int argc, char **argv)
{
	//dlopen(NULL,RTLD_NOW|RTLD_GLOBAL);
	/* for getopt */
	long opt;

	/* run a student name check */
	check_team(argv[0]);

	/* parse the command-line options. For this program, we only support */
	/* the parameterless 'h' option, for getting help on program usage.  */
	while((opt = getopt(argc, argv, "h")) != -1)
	{
		switch(opt)
		{
		case 'h':	help(argv[0]); 	break;
		}
	}

	/* call load_and_invoke() to run the given function of the given library */
	load_and_invoke("./libpart1.so", "hello");

	exit(0);
}

