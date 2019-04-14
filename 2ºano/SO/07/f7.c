#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int n = 0;
int seconds = 0;


void printSIGQUIT(){
	char str[10];
	sprintf(str,"%d\n",n);
	write(1,str,sizeof(str));
}

void printSIGINT(){
	n++;
	char str[15];
	sprintf(str,"Time: %ds\n",seconds);
	write(1,str,strlen(str)*sizeof(char));
}

void printTSTP(){
	char str[20];
	sprintf(str,"Número de ctrl+c: %d\n",n);
	write(1,str,strlen(str)*sizeof(char));
	_exit(0);
}

void mysleep(){
	seconds++;
	alarm(1);
}

void ex1(){
	alarm(1);
	signal(SIGALRM,mysleep);
	signal(SIGINT,printSIGINT);
	signal(SIGTSTP,printTSTP);
	//signal(SIGQUIT,printSIGQUIT);
	while(1) pause();
}

void main(int argc, char** argv){
	ex1();
}

