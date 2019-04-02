#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*int open(const char *path, int oflag [, mode]);
ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);
int close(int fildes);*/

//ex1)
void criaFicheiro(char* argv){
	char* buffer; int f;
	int size = 10500000;
	buffer = malloc(size);

	f = open(argv, O_CREAT | O_WRONLY);
	write(f,buffer,size);
	close(f);
}

//ex2)
void mycat(){
	char c; 

	while(read(0,&c,1))
		write(1,&c,1);
}

//ex3)
void mycatv2(int N){
	char* buffer; int x;
	buffer = malloc(N);

	while(x = read(0,buffer,sizeof(buffer)))
		write(1,buffer,x);
}

//ex4)
//time tp1 1024 < seila.dat > /tmp/lixo

//ex5)
ssize_t readln(int fildes, char* buf, size_t nbyte){
	int size = 0;
	int i = 0; int n = 1;
	char c = 'a';

	while((i<nbyte) && (c != '\n') && (n>0)){
		n = read(fildes,&c,1);
		if(n && c != '\n'){
			buf[i] = c;
			i++;
		}
	}
	return i;
}

//teste do ex5
void println(int argc, char** argv){
	int file = 0; char* buffer;
	int n=0; int size = 30;
	buffer = malloc(size);

	if(argc > 1) file = open(argv[1],O_RDONLY);
	if(file<0) perror(argv[1]);
	else{
		while(n=readln(file,buffer,size)){
			write(1,buffer,n);
			write(1,"\n",1);
		}
	}
	close(file);
}

//ex6
void nl(int argc, char** argv){
	int file = 0; char* buffer;
	int size = 50; int i = 1; int n;
	buffer = malloc(size); char ns[8];

	if(argc > 1) file = open(argv[1],O_RDONLY);
	if(file<0) perror(argv[1]);
	else{
		while(n = readln(file,buffer,size)){
			sprintf(ns,"\t%d  ",i);
			write(1,ns,strlen(ns));
			write(1,buffer,n);
			write(1,"\n",1);
			i++;
		}
	}
	close(file);
}

void main(int argc, char** argv)
{
	//criaFicheiro(argv[1]);
	//mycat();
	//mycatv2(atoi(argv[1]));
	//println(argc,argv);
	nl(argc,argv);
}
