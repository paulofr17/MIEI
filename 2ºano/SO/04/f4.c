#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/*
int dup(int fd); 
int dup2(int fd1, int fd2);*/

void p1(){
	int fi, fo, fe;
	fi = open ("/etc/passwd", O_RDONLY);
	fo = open ("saida.txt", O_CREAT|O_RDWR);
	fe = open ("erros.txt", O_CREAT|O_RDWR);

	dup2(fi,0); //standard input
	dup2(fo,1);	//standard output
	dup2(fe,2);	//standard error

	char buf[100];

	while (read(0, &buf, 100)){
		write (1, &buf, 100);
		write (2, &buf, 100);
	}

	close(fi);
	close(fo);
	close(fe);
}

void p2(){
	int fi, fo, fe;
	fi = open ("/etc/passwd", O_RDONLY);
	fo = open ("saida.txt", O_CREAT|O_RDWR);
	fe = open ("erros.txt", O_CREAT|O_RDWR);

	dup2(fi,0); //standard input
	dup2(fo,1);	//standard output
	dup2(fe,2);	//standard error

	char buf[100];
	int x = fork();

	if(!x)
		while (read(0, &buf, 100)){
			write (1, &buf, 100);
			write (2, &buf, 100);
		}

	close(fi);
	close(fo);
	close(fe);
}

void p3(){
	int fi, fo, fe;
	fi = open ("/etc/passwd", O_RDONLY);
	fo = open ("saida.txt", O_CREAT|O_RDWR);
	fe = open ("erros.txt", O_CREAT|O_RDWR);
	
	dup2(fi, 0);
	dup2(fo, 1);
	dup2(fe, 2);

	close(fi);
	close(fo);
	close(fe);

	char buf[100];

	while (read(0, &buf, 100)){
		write (1, &buf, 100);
		write (2, &buf, 100);
	}
	execlp("wc", "wc", "/etc/passwd", NULL);
}

int main(int argc, char** argv){
	//p1();
	//p2();
	p3();
}