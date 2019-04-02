#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>

/*
pid_t getpid(void);
pid_t getppid(void);
pid_t fork(void);
void _exit(int status);
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
int WIFEXITED(int status); 
int WEXITSTATUS(int status);
*/

void ex1(){
	printf("pai: %d\n",getppid());
	printf("filho: %d\n",getpid());
}

void ex2(){
	int x;
	x = fork();

	if(x){ //pai
		int filho = x;
		//wait(&x); se quiser executar o printf do pai só depois do processo filho terminar 
		printf("pai: %d atual: %d filho: %d\n",getppid(),getpid(),filho);
	}
	else{ //filho
		printf("pai: %d atual: %d\n",getppid(),getpid());
	}
}

void ex3(){
	int i,x,status;

	for(i=1;i<11;i++){
		x = fork();
		if(!x){
			printf("pai - %d atual - %d\n",getppid(),getpid());
			_exit(i);
		}
		else if(x){
			wait(&status);
			printf("Código de saida: %d\n",WEXITSTATUS(status));
		}
	}
}

void ex4(){
	int i,x,status;
	for(i=1;i<11;i++){
		x = fork();
		if(!x){
			printf("pai - %d; atual - %d\n", getppid(), getpid());
			_exit(i);
		}
	}
	for(i=1;i<11;i++){
		if(x){
			wait(&status);
			printf("Código de saida: %d\n",WEXITSTATUS(status));
		}
	}
}

void ex5(){
	int i,status;
	int x = 0;

	for(i=1;i<11 && !x;i++){
		x = fork();
		if(!x){
			printf("pai: %d atual: %d\n",getppid(),getpid());
		}
	}
	if(x) wait(&status);
}

void main(int argc, char** argv){
	//ex1();
	//ex2();
	//ex3();
	//ex4();
	ex5();
}

