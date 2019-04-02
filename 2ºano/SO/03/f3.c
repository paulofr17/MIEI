#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/*
int execl(const char *path, const char *arg0, ..., NULL);
int execlp(const char *file, const char *arg0, ..., NULL);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
*/


void ex1(){
	execlp("ls","ls","-l",NULL);
}

void ex1b(){
	char* ls = "ls";
	char *str[3];
	str[0] = "ls";
	str[1] = "-l";
	str[2] = NULL;
	execvp(ls,str);
}

void ex2(){
	int x = fork(),status;
	if(!x){
		execlp("ls","ls","-l",NULL);
	}
	wait(&status);
}

void ex3(int argc, char** argv){
	int i;
	for(i=1;i<argc;i++)
		printf("%s\n",argv[i]);
}

void ex4(){
	//no ficheiro ex4.c
}

// ./f3 a1 a2 a3
void ex5(int argc, char** argv){
	int i,status;
	for(i=1;i<argc;i++){
		if(!fork()){
			_exit(execlp(argv[i],argv[i],NULL));
		}
	}
	for(i=1;i<argc;i++)
		wait(&status);
}

char** strings(char* s){
	char str[100]; int i = 0;
	char** res = malloc(sizeof(char*) * 10); //10 posições
	strcpy(str,s);
	res[i] = strtok(str," ");
	while(res[i]){
		i++;
		res[i] = strtok(NULL," ");
	}
	/*for(i=0;res[i];i++)
		printf("res[%d]=%s\n",i,res[i]);
	*/
	return res;
}

void ex6(char* command){	
	int i = 0;
	char str[100]; 	
	strcpy(str,command);
	char* res[10];
	char* p = strtok(str," "); 
	//divide a string quando encontra espaços
	while(p!= NULL){
		res[i++] = p;
		p = strtok(NULL," ");
	}
	res[i] = NULL;

	int status, x = fork();
	if(!x){
		_exit(execvp(res[0],res));
	}
	wait(&status);
}

void ex7(){
	char buffer[30];
	int r;
	while(r = read(0,buffer,30) && strcmp(buffer,"exit\n")){
		system(buffer);
		memset(buffer,0,30);
	}
}

void main(int argc, char **argv){
	ex1();
	//ex2();
	//ex3(argc,argv);
	//ex5(argc,argv);
	//if(argc>1) ex6(argv[1]);
	//ex7();
	//system("wc -l abc.txt");
}