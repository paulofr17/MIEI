#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

//signal(sinal,f) substitui o sinal definido por uma função f que defenimos
//os sinais interrompem tudo que está a acontecer e voltam ao local da execução onde o sinal foi chamado
//O SIGKILL/SIGSTOP/SIGCONT não podem ser alterados por funções para segurança do sistema operativo


void f(){
	write(1,"Não. \n",7);
}

void mysleep(int s){
	signal(SIGALRM,f);
	alarm(s);
	pause(); //coloca um processo em pausa até receber um sinal
	signal(SIGALRM,SIG_DFL); //para repor o sinal default
}

int main(int argc, char* argv[]){
	int n;
	char buf[100];

	//signal(SIGINT, f); //tem de ser chamado no inicio do programa
	signal(SIGALRM,mysleep);
	alarm(1);

	while((n=read(0,buf,100))>0)
		write(1,buf,n);

	return 1;
}