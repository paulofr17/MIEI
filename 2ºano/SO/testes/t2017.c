#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

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

int main(int argc, char** argv){
	int i;
	for(i=1; i<argc; i++){ //guarda nos ficheiros o resultados dos execs
		int fdw = open(argv[i], O_CREAT | O_WRONLY, 0644);
		if(!fork()){
			dup2(fdw,1);
			execlp(argv[i],argv[i],NULL);
		}
		else close(fdw);
	}

	int fd[argc], eof[argc] , flag=1, nl=0, n=0;
	for(i=1; i<argc; i++) eof[i] = 0;
	for(i=1; i<argc; i++) fd[i] = open(argv[i], O_RDONLY);
	while(flag){
		for(i=1;i<argc;i++){
			char buf[100], str[20];
			memset(buf,'\0',sizeof(buf));
			while(readln(fd[i],buf,100) && nl<10){
				sprintf(str,"%d %s:\t",n,argv[i]);
				write(1,str,strlen(str));
				write(1,buf,strlen(buf));
				write(1,"\n",strlen("\n"));
				n++;nl++;
				memset(buf,'\0',sizeof(buf));
			}
			if(nl<10) eof[i] = 1; //fim do ficheiro
			else eof[i] = 0; //nao chegou ao fim
			nl = 0; //contador de linhas para 0
		}
		flag = 0;
		for(i=1;i<argc;i++)
			if(!eof[i]) flag = 1; //caso algum deles nao esteja no EOF
	}
	for(i=1; i<argc; i++){ 
		close(fd[i]);
	}
	return 0;
}

