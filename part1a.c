#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"support.h"
#include<string.h>
/* help() - Print a help message. */
void help(char *progname)
{
	printf("Usage: %s\n", progname);
	printf("(This is just a demonstration of the traditional way\n");
	printf(" to use .so filed)\n");
}

/* declare the hello() function that lives in a shared library */
extern void *hello(void* param);
extern void *ucase(struct team_t* ut,struct team_t* teamo);
/* main() - The main routine parses arguments and invokes hello */
int main(int argc, char **argv)
{
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
		case 'h': 	help(argv[0]); 	break;
		}
	}

	hello(NULL);

	/* TODO: execute the new function "ucase" that you added to libpart1.c */

	struct team_t ut;
	ucase(&ut,&team);
	
    if((strcmp("", ut.name1) == 0) || (strcmp("", ut.email1) == 0))
	{
		printf("Please fill in the team struct in team.c\n");
		exit(1);
	}
	if((strcmp("", ut.name2) == 0) || (strcmp("", ut.email2) == 0))
	{
		printf("Please fill in the team struct in team.c\n");
		exit(1);
	}
	if((strcmp("", ut.name3) == 0) || (strcmp("", ut.email3) == 0))
	{
		printf("Please fill in the team struct in team.c\n");
		exit(1);
	}
	printf("Student 1 : %s\n", ut.name1);
	printf("Email 1   : %s\n", ut.email1);
	printf("Student 2 : %s\n", ut.name2);
	printf("Email 2   : %s\n", ut.email2);
	printf("Student 3 : %s\n", ut.name3);
	printf("Email 3   : %s\n", ut.email3);
	printf("\n");
	free(ut.name1);
	free(ut.name2);
	free(ut.name3);
	free(ut.email1);
	free(ut.email2);
	free(ut.email3);
	return 0;
}
