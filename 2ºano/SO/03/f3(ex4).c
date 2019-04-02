#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void main(int argc, char** argv)
{
	execv("./f3",argv);
}