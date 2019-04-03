#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>

void ex1(){
	char c;
	int pd[2];
	pipe(pd);

	if(!fork()){
		close(pd[1]);
		while(read(pd[0],&c,1))
			write(1,&c,1);
		close(pd[0]);
	}
	else{
		sleep(5); //ex1.1(é só adicionar sleep(5))
		close(pd[0]);
		write(pd[1],"pai\n",sizeof("pai\n"));
		close(pd[1]);
	}
}

void ex1_2(){
	char c;
	int pd[2];
	pipe(pd);

	if(!fork()){
		close(pd[0]);
		write(pd[1],"filho\n",sizeof("filho\n"));
		close(pd[1]);
	}
	else{
		close(pd[1]);
		while(read(pd[0],&c,1))
			write(1,&c,1);
		close(pd[0]);
	}
}

//ex2(é o mesmo que o ex1_2, para verificar basta tirar o close(pd[1]) antes do while(read...) e o programa fica em deadlock)

void ex3(){
	char c;
	int pd[2];
	pipe(pd);

	if(!fork()){
		close(pd[1]);
		dup2(pd[0],0);
		execlp("wc","wc",NULL);
		close(pd[0]);
	}
	else{
		close(pd[0]);
		while(read(0,&c,1))
			write(pd[1],&c,1);
		close(pd[1]);
	}
}

void ex4(){
	int pd[2];
	pipe(pd);

	if(!fork()){
		close(pd[1]);
		dup2(pd[0],0);
		execlp("wc","wc","-l",NULL);
		close(pd[0]);
	}
	else{
		close(pd[0]);
		dup2(pd[1],1);
		execlp("ls","ls","/etc",NULL);
		close(pd[1]);
	}
}


//grep -v ˆ# /etc/passwd | cut -f7 -d: | uniq | wc -l
void ex5(){
	int pd[2];
	pipe(pd);

	if(!fork()){
		close(pd[0]);
		dup2(pd[1],1);
		execlp("grep","grep","-v","^#","/etc/passwd",NULL);
	}
	else{
		int pd1[2];
		pipe(pd1);
		close(pd[1]);
		if(!fork()){
			dup2(pd[0],0);
			dup2(pd1[1],1);
			execlp("cut","cut","-f7","-d:",NULL);
		}
		else{
			int pd2[2];
			pipe(pd2);
			close(pd1[1]);
			if(!fork()){
				dup2(pd1[0],0);
				dup2(pd2[1],1);
				execlp("uniq","uniq",NULL);
			}
			else{
				close(pd2[1]);
				dup2(pd2[0],0);
				execlp("wc","wc","-l",NULL);
			}
		}
	}
}

void main(){
	//ex1();
	//ex1_2();
	//ex3();
	//ex4();
	ex5();
}

