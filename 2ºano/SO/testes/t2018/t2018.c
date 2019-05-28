#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

void f(int s){
	kill(getpid(),SIGKILL);
}

int main(int argc, char** argv){
	signal(SIGALRM,f);
	int pid[argc], x, status[argc], i, j, sec = 0;

	while(1){
		for(i=1;i<argc;i++){
			pid[i] = fork();
			if(!pid[i]){
				_exit(execlp(argv[i],argv[i],NULL));
			}
		}

		for(i=1;i<argc;i++){
			if(pid[i]){
				wait(&status[i]);
				if(WEXITSTATUS(status[i])==0){
					for(i=1;i<argc;i++)
						kill(pid[i],SIGKILL);
					_exit(0);
				}
			}
		}
		sec = 0;
		for(i=1;i<argc;i++) sec+=WEXITSTATUS(status[i]);
		alarm(sec);
	}
}