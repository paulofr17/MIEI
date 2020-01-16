#include "estrutura.h"
/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem das cartas
*/
#define VALORES		"3456789TJQKA2"
/**
URL da CGI
*/
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"

/** \brief Imprime o html correspondente a uma carta
@param path	o URL correspondente à pasta que contém todas as cartas
@param x        A coordenada x da carta
@param y        A coordenada y da carta
@param e	    O estado atual
@param mao      Número do jogador
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/

void imprime_carta(char *path, int x, int y, ESTADO e, int mao, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
    if (mao == 0){
		ESTADO novo = e;
		if (carta_existe(novo.highlight,naipe,valor))
		novo.highlight = rem_carta (novo.highlight,naipe,valor);
	    else
		novo.highlight = add_carta (novo.highlight,naipe,valor);
		sprintf(script, "%s?%s", SCRIPT, estado2str(novo));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"186\" width = \"135\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);

    }
    else printf("<image x = \"%d\" y = \"%d\" height = \"186\" width = \"135\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, path, rank[valor], suit[naipe]);
}


/** \brief Imprime o botão para jogar
@param path    o URL correspondente à pasta que contém todas as cartas
@param e       Estado atual
*/

void imprime_jogar (char *path, ESTADO e){
	char script[10240];
	int x=900,y=800;
	e.inicio = inicio(e);
	if ((posso_jogar(e) == 1 || (e.inicio == 1 && jogador_inicial(e) != 0)) && (conta_cartas(e.mao[e.ultimo_jogador]) != 0)){
		ESTADO jogar = e;
		jogar.jogar = 1;
		sprintf(script, "%s?%s", SCRIPT, estado2str(jogar));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"85\" width = \"85\" xlink:href = \"%s/jogar_on.svg\" value=\"PLAY\" onclick=\"play(jogar)\" /></a>\n", script, x, y, path);
	}
	else
		printf("<image x = \"%d\" y = \"%d\" height = \"85\" width = \"85\" xlink:href = \"%s/jogar_off.svg\" /></a>\n", x, y, path);
}


/** \brief Imprime o botão para passar
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_passar (char *path, ESTADO e){
	char script[10240];
	int x = 1000, y = 800;
	if (conta_cartas(e.mao[e.ultimo_jogador]) == 0 || e.ultimo_jogador == 0) printf("<image x = \"%d\" y = \"%d\" height = \"85\" width = \"85\" xlink:href = \"%s/passar_off.svg\"  /></a>\n", x, y, path);
	else{
		ESTADO passar = e;
		passar.passar = 1;
		sprintf(script, "%s?%s", SCRIPT, estado2str(passar));
		printf("<a xlink:href = \"%s\"><image x = \"1000\" y = \"800\" height = \"85\" width = \"85\" xlink:href = \"%s/passar_on.svg\" value=\"PLAY\" onclick=\"play(passar)\" /></a>\n", script, path);
    }
}


/** \brief Imprime o botão para limpar a seleção
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_limpar (char *path, ESTADO e){
	char script[10240];
	int x = 1100, y = 795;
    ESTADO limpar = e;
	limpar.limpar = 1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(limpar));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"50\" width = \"50\" xlink:href = \"%s/limpar.svg\" value=\"PLAY\" onclick=\"play(limpar)\" /></a>\n", script, x, y, path);
}


/** \brief Imprime o botão para a próxima ronda
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_proxima_ronda (char *path, ESTADO e){
	int x = 1300, y = 400;
	char script[10240];
	if (conta_cartas (e.mao[e.ultimo_jogador]) == 0){
		ESTADO proxima_ronda = e;
		proxima_ronda.proxima_ronda = 1;
		sprintf(script, "%s?%s", SCRIPT, estado2str(proxima_ronda));
    	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"180\" width = \"180\" xlink:href = \"%s/restart_on.svg\" value=\"PLAY\" onclick=\"play(proxima_ronda)\" /></a>\n", script, x, y, path);
    }
    else
    	printf("<image x = \"%d\" y = \"%d\" height = \"180\" width = \"180\" xlink:href = \"%s/restart_off.svg\" /></a>\n", x, y, path);
}


/** \brief Imprime o botão para ordenar as cartas
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_ordena (char *path, ESTADO e){
	int x = 1100, y = 845;
	char script[10240];
    ESTADO ordena = e;
    ordena.ordena = 1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(ordena));
    printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"50\" width = \"50\" xlink:href = \"%s/ordena.svg\" value=\"PLAY\" onclick=\"play(ordenar)\" /></a>\n", script, x, y, path);
}


/** \brief Imprime a pontuação atual
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_pontuacao(ESTADO e){
	printf ("<text x=\"1270\" y=\"180\" text-decoration = \"underline\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"bold\"> Pontuações</text>");
	printf ("<text x=\"1295\" y=\"240\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"\">Sul.......%d</text>", e.pontuacao[0]);
	printf ("<text x=\"1295\" y=\"290\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"\">Oeste...%d </text>", e.pontuacao[1]);
	printf ("<text x=\"1295\" y=\"340\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"\">Norte...%d</text>", e.pontuacao[2]);
	printf ("<text x=\"1295\" y=\"390\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"\">Este.....%d</text>", e.pontuacao[3]);
}


/** \brief Imprime o botão para a sugestão
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_sugestao(char *path, ESTADO e){
	char script[10240];
	ESTADO sugestao = e;
	sugestao.sugestao = 1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(sugestao));
	printf("<a xlink:href = \"%s\"><image x = \"1100\" y = \"900\" height = \"50\" width = \"50\" xlink:href = \"%s/sugestao.svg\" value=\"PLAY\" onclick=\"play(sugestao)\" /></a>\n", script, path);
}


/** \brief Imprime o botão para o novo jogo (ou seja, recarregar a página)
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_novo_jogo(char *path){
	printf("<a xlink:href = \"http://127.0.0.1/cgi-bin/cartas\"><image x = \"1100\" y = \"970\" height = \"50\" width = \"50\" xlink:href = \"%s/voltar.svg\" /></a>\n", path);
}


/** \brief Imprime as conquistas (i.e. o que o jogador já conseguiu alcançar)
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_conquistas(ESTADO e){
	int i,y=735;
	char conquistas[9][30] = {"Jogar um par", "Jogar um triplo", "Jogar um Straight", "Jogar um Flush", "Jogar um Full House", "Jogar um Four of a Kind", "Jogar um Straight Flush", "Ganhar uma ronda", "Ganhar um jogo"};
	for (i=0;i<9;i++){
		if (carta_existe (e.conquistas, 0, i))
			 printf ("<text x=\"1230\" y=\"%d\" fill=\"#00FF00\" text-anchor = \"left\" text-align = \"center\" font-size = \"30\" font-weight = \"bold\" >✔</text>", y);
		else printf ("<text x=\"1230\" y=\"%d\" fill=\"#FF0000\" text-anchor = \"left\" text-align = \"center\" font-size = \"30\" font-weight = \"bold\" >✘</text>", y);
		y += 35;
	}
	printf ("<text x=\"1240\" y=\"675\" text-decoration = \"underline\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"50\" font-weight = \"bold\">Conquistas</text>");
	
	y = 730;
	for(i=0;i<9;i++){
		printf ("<text x=\"1260\" y=\"%d\" text-decoration = \"underline\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"30\" font-weight = \"\">%s</text>", y, conquistas[i]);
		y += 35;
	}
}


/* \brief função que se encarrega de imprimir os nomes dos jogadores, as linhas, o título e o número do jogo
*/
void imprime_UI(char *imagens, ESTADO e){
   printf("<image x = \"%d\" y = \"%d\" height = \"30\" width = \"135\" xlink:href = \"%s/norte.svg\" /></a>\n", 210, 25, imagens);
   printf("<image x = \"%d\" y = \"%d\" height = \"30\" width = \"135\" xlink:href = \"%s/sul.svg\" /></a>\n", 230, 850, imagens);
   printf("<image x = \"%d\" y = \"%d\" height = \"30\" width = \"135\" xlink:href = \"%s/este.svg\" /></a>\n", 950, 215, imagens);
   printf("<image x = \"%d\" y = \"%d\" height = \"30\" width = \"135\" xlink:href = \"%s/oeste.svg\" /></a>\n", 35, 215, imagens);
   printf("<rect x=1180 y=610 width=500 height=10 ry=0 text-outline = \"20\" fill = \"rgb(0,0,0)\" />");
   printf("<rect x=1180 y=0 width=10 height=1600 ry=0 text-outline = \"20\" fill = \"rgb(0,0,0)\" />");
   printf ("<text x=\"1230\" y=\"100\" fill = \"#ff0000\"  text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"100\" font-weight = \"bold\"> BigTwo</text>");
   printf("<text x=\"1440\" y=\"135\"  text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"20\"> Jogo nº%d</text>", e.numero_jogo);	
}


/** \brief Imprime uma carta fornecida pela função imprime
@param e 		Estado atual
@param path 	o URL correspondente à pasta que contém todas as cartas
@param m 		Número da mão onde se quer imprimir a carta
@param n 		Naipe da carta
@param v 		Valor da carta
@param x 		Posição do eixo x
@param y 		Posição do eixo y
*/

void imprime_aux(ESTADO e, char *path, int m, int n, int v, int x, int y){
	int Y = y;
	if (m == 0 && carta_existe(e.highlight, n , v))
		Y -= 20;
	imprime_carta(path, x, Y, e, m, n, v);
}


/** \brief Imprime as cartas
Irá imprimir tanto as cartas das mãos como as que foram jogadas
@param path	o URL correspondente à pasta que contém todas as cartas
@param e	O estado atual
*/

void imprime_cartas(char *baralho, ESTADO e) {
	int n,v,m,x,y;
	int px[4] = {330,50,330,960};	int py[4] = {760,250,20,250};
	int dx[4] = {25,0,25,0};		int dy[4] = {0,23,0,23};
	int mx[4] = {470,230,470,710};	int my[4] = {550,380,230,380};
    for(m=0;m<4;m++){
		   x=px[m]; y=py[m];
		   if (e.ordena_cartas == 0){
	           for(v = 0; v < 13; v++)
				   for(n = 0; n < 4; n++)
						if(carta_existe(e.mao[m], n, v))
							imprime_aux(e, baralho, m, n, v, x = x + dx[m], y = y + dy[m]);
		   }
			else for(n = 0; n < 4; n++)
					for(v = 0; v < 13; v++) 
						if(carta_existe(e.mao[m], n, v))
							imprime_aux(e, baralho, m, n, v, x = x + dx[m], y = y + dy[m]);
			x = 0;
   	  		for(v = 0; v < 13; v++)
	     		for(n = 0; n < 4; n++)
		     		if(carta_existe(e.cartas_na_mesa[m], n, v)) {
		     			imprime_carta(baralho, mx[m] + x, my[m], e ,-1 ,n, v);
		     			x += 25;
             		}
	}
}


/** \brief Função se encarrega de chamar as funções que irão imprimir as diversas partes do jogo
@param baralho Url com as imagens das cartas
@param imagens Url com as imagens dos botões, fundo, títulos ...
@param e Estado atual
*/

void imprime(char *baralho, char *imagens, ESTADO e) {
   imprime_cartas(baralho, e);
   imprime_UI(imagens, e);
   imprime_limpar(imagens, e);
   imprime_jogar(imagens, e);
   imprime_passar(imagens, e);
   imprime_proxima_ronda(imagens, e);
   imprime_pontuacao(e);
   imprime_ordena(imagens, e);
   imprime_sugestao(imagens, e);
   imprime_novo_jogo(imagens);
   imprime_conquistas(e);
}



/** \brief Imprime o menu inicial com os respetivos botões
@param path    o URL correspondente à pasta que contém todas as imagens
@param e       Estado atual
*/

void imprime_menu(char *path, ESTADO e){
	char script_jogar[10240];
	char script_instrucoes[10240];
	ESTADO menu_jogar = e;
	ESTADO menu_instrucoes = e;
	printf("<image x = \"350\" y = \"50\" height = \"626\" width = \"957\" xlink:href = \"%s/titulo.png\" /></a>\n", path);
	menu_jogar.menu_jogar = 1;
	sprintf(script_jogar, "%s?%s", SCRIPT, estado2str(menu_jogar));
	printf("<a xlink:href = \"%s\"><image x = \"680\" y = \"580\" height = \"150\" width = \"400\" xlink:href = \"%s/menu_jogar.svg\" /></a>\n", script_jogar, path);
	menu_instrucoes.menu_instrucoes = 1;
	sprintf(script_instrucoes, "%s?%s", SCRIPT, estado2str(menu_instrucoes));
	printf("<a xlink:href = \"%s\"><image x = \"600\" y = \"680\" height = \"150\" width = \"400\" xlink:href = \"%s/menu_instrucoes.svg\" /></a>\n", script_instrucoes, path);
	printf("<a target= \"_blank\" xlink:href = \"https://pt.wikipedia.org/wiki/Big_Two\"><image x = \"600\" y = \"780\" height = \"150\" width = \"400\" xlink:href = \"%s/menu_wikipedia.svg\" /></a>\n", path);

	printf("<a target= \"_blank\" xlink:href = \"http://www.wikihow.com/Play-Big-Two\"><image x = \"620\" y = \"880\" height = \"150\" width = \"400\" xlink:href = \"%s/menu_wikihow.svg\" /></a>\n", path);
}


/* \brief Irá encarregar-se de atribuir a cada variável um som
*/ 
void som(){
	printf("<audio id=\"jogar\" src=\"http://127.0.0.1/files/jogar_som.wav\" ></audio>");
	printf("<audio id=\"passar\" src=\"http://127.0.0.1/files/passar_som.wav\" ></audio>");
	printf("<audio id=\"proxima_ronda\" src=\"http://127.0.0.1/files/proxima_ronda_som.wav\" ></audio>");
	printf("<audio id=\"ordenar\" src=\"http://127.0.0.1/files/ordenar_som.wav\" ></audio>");
	printf("<audio id=\"sugestao\" src=\"http://127.0.0.1/files/sugestao_som.wav\" ></audio>");
	printf("<audio id=\"limpar\" src=\"http://127.0.0.1/files/limpar_som.wav\" ></audio>");
}


/** \brief Imprime cabeçalhos necessários numa CGI, a janela e o script para o som
*/
void imprime_janela_e_scripts(){
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>BigTwo</title></header>\n");
	printf("<script> function play(x){ var som = document.getElementById(\"x\"); x.play(x); } </script>");
	printf("<body background=\"http://127.0.0.1/files/background.png\">\n");
	printf("<svg height = \"1025\" width = \"1600\">\n");
}