#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

//signal(sinal,f) substitui o sinal definido por uma função f que defenimos
//os sinais interrompem tudo que está a acontecer e voltam ao local da execução onde o sinal foi chamado
//O SIGKILL/SIGSTOP/SIGCONT não podem ser alterados por funções para segurança do sistema operativo

int n = 0;
int seconds = 0;


void printSIGQUIT(){
	char str[30];
	sprintf(str," Número de ctrl+c: %d\n",n);
	write(1,str,strlen(str));
	_exit(0);
}

void printSIGINT(){
	n++;
	char str[30];
	sprintf(str," Time: %ds\n",seconds);
	write(1,str,strlen(str));
}

void printTSTP(){
	char str[30];
	sprintf(str,"Número de ctrl+c: %d\n",n);
	write(1,str,strlen(str));
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
	signal(SIGQUIT,printSIGQUIT);
	//signal(SIGTSTP,printTSTP);
	while(1) pause(); //coloca um processo em pausa até receber um sinal
}

void main(int argc, char** argv){
	ex1();
}

