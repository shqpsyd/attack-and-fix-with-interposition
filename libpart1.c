#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"support.h"

/* hello() - print some output so we know the shared object loaded.
 *
 * The function signature takes an untyped parameter, and returns an untyped
 * parameter. In this way, the signature *could* support any behavior, by
 * passing in a struct and returning a struct. */
void *hello(void *input)
{
	printf("hello from a .so\n");
	return NULL;
}
void strupr(char* dest,char * source) {
	for (int i = 0; i < strlen(source); i++) {
		dest[i] = toupper(source[i]);
	}
}

void *ucase(struct team_t* ut,struct team_t* teamo){
		
	ut->name1 = malloc(strlen(teamo->name1));
	strupr(ut->name1,teamo->name1);
	ut->name2 = malloc(strlen(teamo->name2));
	strupr(ut->name2,teamo->name2);
	ut->name3 = malloc(strlen(teamo->name3));
	strupr(ut->name3,teamo->name3);
	ut->email1 = malloc(strlen(teamo->email1));
	strupr(ut->email1,teamo->email1);
	ut->email2 = malloc(strlen(teamo->email2));
	strupr(ut->email2,teamo->email2);
	ut->email3 = malloc(strlen(teamo->email3));
	strupr(ut->email3,teamo->email3);
	return NULL;
}

