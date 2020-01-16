#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interface.h"
/**
URL da pasta com as cartas
*/
#define BARALHO		"http://127.0.0.1/cards"
/**
URL da pasta com as imagens
*/
#define FILES     "http://127.0.0.1/files"


/** \brief Distribui as cartas aleatoriamente pelas 4 mãos.
Começa no 3 de ouros e acaba no 2 de espadas.
Vai atribuindo uma a uma a um jogador aleatório, se este tiver menos de 13 cartas.
@param e Estado atual
@return Estado atualizado
*/

ESTADO distribuir(ESTADO e){
	int naipe,valor,x;
	int cartas[4] = {0};
	srand (time(NULL));
	for (valor=0;valor<=12;valor++){
		for (naipe=0;naipe<=3;naipe++) {
			do x = rand() % 4;
			while (cartas[x] >= 13); {
              e.mao[x] = add_carta(e.mao[x],naipe,valor);
              cartas[x] ++;
            }
	    }
	}
	return e;
}


/** \brief Irá jogar uma carta maior que a jogada anterior, caso exista
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO uma_carta(ESTADO e, int m){
	int v,n;
	MAO uma_carta = 0;
	v = 0; n = 0;
	if (e.ultima_jogada != 0){
		v = valor_mao(e.ultima_jogada);
		n = naipe_mao(e.ultima_jogada);
	}
	for(;v<13;v++){
		for (;n<4;n++)
			if (carta_existe(e.mao[m],n,v)){
				uma_carta = add_carta(uma_carta,n,v);
				return uma_carta;
				}
		n = 0;
	}
	return 0;
}


/** \brief Irá jogar um par maior que o par anterior, caso exista
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO duas_cartas(ESTADO e, int m){
	int v,n1,n2;
	MAO duas_cartas = 0;
	if (e.ultima_jogada != 0)
		v = valor_mao(e.ultima_jogada);
	else v = 0;
	for (;v<13;v++)
		for (n1=0;n1<3;n1++)
			for (n2=n1+1;n2<4;n2++)
				if ( carta_existe(e.mao[m], n1, v) && carta_existe(e.mao[m], n2, v) 
					&& (v > valor_mao(e.ultima_jogada) || (valor_mao(e.ultima_jogada) == v && n2 > naipe_mao(e.ultima_jogada))) ){
					duas_cartas = add_carta((add_carta(duas_cartas,n1,v)),n2,v);
					if (e.inicio && !(carta_existe(duas_cartas,0,0)))
						return 0;
					return duas_cartas;
				}
    return 0;
}


/** \brief Irá jogar um triplo maior que a ultimo triplo, caso exista
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO tres_cartas(ESTADO e, int m){
	int v,n1,n2,n3;
	MAO tres_cartas = 0;
	if (e.ultima_jogada != 0)
		v = valor_mao(e.ultima_jogada) +1;
	else v = 0;
	for (;v<13;v++)
		for (n1=0;n1<2;n1++)
			for (n2=n1+1;n2<3;n2++)
				for (n3=n2+1;n3<4;n3++)
					if (carta_existe(e.mao[m], n1, v) && carta_existe(e.mao[m], n2,v) && carta_existe(e.mao[m], n3, v)){
						tres_cartas = add_carta ((add_carta((add_carta(tres_cartas,n1,v)),n2,v)), n3, v);
						if (e.inicio && !(carta_existe(tres_cartas,0,0)))
							return 0;
						return tres_cartas;
					}
	return 0;
}


/** \brief Irá jogar um straight flush maior que o anterior, caso exista
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO straightflush (ESTADO e, int m){
	int v,n,v1,v2,v3,v4,v5;
	MAO straightflush = 0;
	n = 0; v = -2;
	if (tipo_combinacao(e.ultima_jogada) == 5){
		    n = naipe_mao(e.ultima_jogada)+1;
			v = valor_mais_baixo_straight(e.ultima_jogada);
	}
	if (v == -2) {v1 = 11; v2 = 12;};
	if (v == -1) {v1 = 12; v2 = 0;};
	if (v != -1 && v != -2) {v1 = v; v2 = v+1;};
	for (v3 = v+2, v4 = v+3, v5 = v+4; v<8; v++,v1++,v2++,v3++,v4++,v5++){
    	for(;n<4;n++){
    		printf("n: %d\n",n);
    		if (carta_existe(e.mao[m], n, v1) && carta_existe(e.mao[m], n, v2) && carta_existe(e.mao[m], n, v3) 
    			&& carta_existe(e.mao[m], n, v4) && carta_existe(e.mao[m], n, v5)){
   				straightflush = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (straightflush, n, v1)), n, v2)), n,v3)), n,v4)), n,v5);
   				if (e.inicio && !(carta_existe(straightflush,0,0)))
					return 0;
				return straightflush;
			}
    	}
  		n = 0;
  	    if (v2 == 12) v2 = -1;
    	if (v1 == 12) v1 = -1;
  	}
	return 0;
}


/** \brief Irá jogar um four of a kind maior que a combinação anterior, caso exista
Caso não exista, irá tentar jogar um straight flush
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO fourofakind (ESTADO e, int m){
	int vq,v,n;
	MAO fourofakind = 0;
	if (tipo_combinacao(e.ultima_jogada) == 4)
		vq = valor_triplo_ou_quarto(e.ultima_jogada) +1;
	else vq = 0;
	for (;vq<13;vq++)
		if (carta_existe(e.mao[m], 0, vq) && carta_existe(e.mao[m], 1,vq) && carta_existe(e.mao[m], 2, vq) && carta_existe(e.mao[m], 3, vq))
			for (v=0;v<13;v++){
				if (v == vq) v++;
				for (n=0;n<4;n++)
					if (carta_existe(e.mao[m], n, v)){
						fourofakind = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (fourofakind, 0, vq)), 1, vq)), 2,vq)), 3,vq)), n,v);
						if (e.inicio && !(carta_existe(fourofakind,0,0)))
							return straightflush(e,m);
						return fourofakind;
					}
			}
	return straightflush(e,m);
}


/** \brief Irá jogar um full house maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um four of a kind
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO fullhouse (ESTADO e, int m){
	int vt,nt1,nt2,nt3,vp,np1,np2; MAO fullhouse = 0;
	if (tipo_combinacao (e.ultima_jogada) == 3)
		vt = valor_triplo_ou_quarto(e.ultima_jogada) +1;
	else vt = 0;
	for (;vt<13;vt++)
		for (nt1=0;nt1<2;nt1++)
			for (nt2=nt1+1;nt2<3;nt2++)
				for (nt3=nt2+1;nt3<4;nt3++)
					if (carta_existe(e.mao[m], nt1, vt) && carta_existe(e.mao[m], nt2,vt) && carta_existe(e.mao[m], nt3, vt))
						for (vp=0;vp<13;vp++){
							if (vp == vt) vp++;
							for (np1=0;np1<3;np1++)
								for (np2=np1+1;np2<4;np2++)
									if (carta_existe(e.mao[m], np1, vp) && carta_existe(e.mao[m], np2,vp)){
										fullhouse = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (fullhouse, nt1, vt)), nt2, vt)), nt3,vt)), np1,vp)), np2,vp);
										if (e.inicio && !(carta_existe(fullhouse,0,0)))
												return fourofakind(e,m);
										return fullhouse;
									}
							if (vp == 12) return 0;
						}
	return fourofakind(e,m);
}


/** \brief Irá jogar um flush maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um full house
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO flush (ESTADO e, int m){
	int n,v,c, valores[5] = {0};
	MAO flush = 0;
	if (tipo_combinacao(e.ultima_jogada) == 2)
		n = naipe_mao(e.ultima_jogada);
	else n = 0;
	for (c=0;n<4 && c<5;n++){
		c = 0;
		for (v=0;v<13 && c<5;v++)
			if (carta_existe(e.mao[m], n, v))
				if(c!=4 || n > naipe_mao(e.ultima_jogada) || v > valor_mais_alto(e.ultima_jogada)){
						valores[c] = v;
						c++;
				}
	}
	if (c == 5){
		n--;
		flush = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (flush, n, valores[0] )), n, valores[1])), n,valores[2])), n,valores[3])), n,valores[4]);
			if (e.inicio && !(carta_existe(flush,0,0)))
				return fullhouse(e,m);
		return flush;
	}
	return fullhouse(e,m);
}


/** \brief Irá jogar um straight maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um flush
@param e Estado
@param m Número do jogador
@return Jogada
*/

MAO straight (ESTADO e, int m){
	int v = valor_mais_baixo_straight(e.ultima_jogada);
	int v1,v2,v3,v4,v5,n5;
	MAO straight = 0;
	if (e.inicio) v = -2;
	if (v == -2) {v1 = 11; v2 = 12;};
	if (v == -1) {v1 = 12; v2 = 0;};
	if (v != -1 && v != -2) {v1 = v; v2 = v+1;};
	for(v3 = v+2, v4 = v+3, v5 = v+4; v<8;v++,v1++,v2++,v3++,v4++,v5++){
		if (v == valor_mais_baixo_straight(e.ultima_jogada) && !(e.inicio))
			n5 = naipe_maior_carta_straight(e.ultima_jogada)+1;	 	
		else n5 = naipe_mais_baixo_v(e.mao[m], v5);
		for (;n5<4; n5++)
			if (valores_existem(e.mao[m], v1, v2, v3, v4, v5) && carta_existe(e.mao[m], n5, v5)){
				int n1 = naipe_mais_baixo_v(e.mao[m], v1);
				int n2 = naipe_mais_baixo_v(e.mao[m], v2);
				int n3 = naipe_mais_baixo_v(e.mao[m], v3);
				int n4 = naipe_mais_baixo_v(e.mao[m], v4);
				straight = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (straight, n1, v1)), n2, v2)), n3,v3)), n4,v4)), n5,v5);
				if (e.inicio && !(carta_existe(straight,0,0))){
					return flush(e, m);
				}
	    		return straight;
    		}
    	if (v2 == 12) v2 = -1;
    	if (v1 == 12) v1 = -1;
    }
    return flush(e, m);
}


/** \brief Joga uma combinação (i.e. remove as cartas da mão do jogador atual, atualiza a ultima jogada e o ultimo jogador e coloca a jogada para imprimir)
@param e Estado atual
@param Jogada Jogada a jogar
@param m Jogador atual
@return Estado atualizado
*/

ESTADO joga_combinacao(ESTADO e, MAO jogada, int m){
	if (jogada == 0)
		return e;
	e.ultima_jogada = jogada;
	e.mao[m] -= jogada;
	e.ultimo_jogador = m;
	e.cartas_na_mesa[m] = e.ultima_jogada;
	return e;
}


/** \brief Irá verificar qual é a combinação de 5 cartas anterior e irá chamar a função respetiva para jogar uma combinação maior
@param e Estado
@param m Número do jogador
@return Estado atualizado
*/

MAO cinco_cartas (ESTADO e, int m){
	if (tipo_combinacao(e.ultima_jogada) == 5)
		return straightflush(e, m);
	if (tipo_combinacao(e.ultima_jogada) == 4)
		return fourofakind(e, m);
	if (tipo_combinacao(e.ultima_jogada) == 3)
		return fullhouse(e, m);
	if (tipo_combinacao(e.ultima_jogada) == 2)
		return flush(e, m);
	if (tipo_combinacao(e.ultima_jogada) == 1)
		return straight(e,m);
	return 0;
}


/** \brief Irá jogar uma combinação inicial (i.e. quando o bot for livre de jogar o que quiser)
@param e Estado
@param m Número do jogador inicial
@return Estado atualizado
*/

ESTADO bots_inicio(ESTADO e, int m){
	MAO jogada = 0;
	e.highlight = 0;
	e.ultima_jogada = 0;
	jogada = straight(e, m);
	if (jogada == 0) jogada = tres_cartas(e, m);
	if (jogada == 0) jogada = duas_cartas(e, m);
	if (jogada == 0) jogada = uma_carta(e, m);
	e.inicio = 0;
	e = joga_combinacao(e,jogada,m);
	return e;
}


/** \brief Função auxiliar de jogam_bots que irá encarregar-se de fornecer uma jogada consoante a jogada anterior
@param e Estado atual
@param m Número do jogador
@return Estado atualizado
*/

ESTADO jogam_botsAux(ESTADO e, int m){
	int numero_cartas_ultima_jogada = conta_cartas(e.ultima_jogada);
	MAO jogada = 0;
	if (numero_cartas_ultima_jogada == 1)
		jogada = uma_carta(e, m);
	if (numero_cartas_ultima_jogada == 2)
		jogada = duas_cartas(e, m);
	if (numero_cartas_ultima_jogada == 3)
		jogada = tres_cartas(e, m);
	if (numero_cartas_ultima_jogada == 5)
		jogada = cinco_cartas(e, m);
	e = joga_combinacao(e, jogada, m);
	return e;
}


/** \brief Função que permite que os bots joguem
Esta função irá analisar qual é o número de cartas jogadas anteriormente e irá jogar uma mão válida.
Se não tiverem cartas válidas para jogar, passa a vez para o próximo jogador.
@param e Estado atual
@return Estado atualizado
*/

ESTADO jogam_bots (ESTADO e){
	int m=1;
	if (conta_cartas (e.mao[0]) == 0){
		e.conquistas = add_carta(e.conquistas, 0, 7);
		return e;
	}
	if (e.inicio == 1){
		m = jogador_inicial(e);
		e = bots_inicio(e, m);
		m++;
 	}
	for (;m<4;m++){
		e.cartas_na_mesa[m] = 0;
		if (e.ultimo_jogador == m){
			e = limpa_cartas_mesa(e);
			e = bots_inicio(e,m);
			m++;
		}
		if (e.mao[e.ultimo_jogador] == 0)
			return e;
		e = jogam_botsAux(e,m);
    }
    return e;
}


/** \brief Irá verificar se o jogador desbloqueou alguma conquista (por exemplo, se jogou um straight)
@param e Estado
@return Estado atualizado
*/

ESTADO verifica_conquistas(ESTADO e){
	int t = tipo_combinacao(e.highlight);
	int c = conta_cartas(e.highlight);
	if (c == 2) e.conquistas = add_carta(e.conquistas, 0, 0);
	if (c == 3) e.conquistas = add_carta(e.conquistas, 0, 1);
	if (c == 5){
		if (t == 1) e.conquistas = add_carta(e.conquistas, 0, 2);
		if (t == 2) e.conquistas = add_carta(e.conquistas, 0, 3);
		if (t == 3) e.conquistas = add_carta(e.conquistas, 0, 4);
		if (t == 4) e.conquistas = add_carta(e.conquistas, 0, 5);
		if (t == 5) e.conquistas = add_carta(e.conquistas, 0, 6);
	}
	return e;
}


/** \brief Função que permite que as cartas sejam jogadas
Irá analisar quem tem o 3 de ouros e se uma rodada chegou ao fim (i.e. passaram 3 jogadores seguidos)
Quem tiver o 3D jogará primeiro.
Controla também o jogo do utilizador. Se tiver cartas selecionadas (validadas) irá joga-las e passar a vez para os outros 3 jogadores.
@param e Estado atual
*/

ESTADO jogar(ESTADO e){
	int n,v;
	e.jogar = 0; 
	e.cartas_na_mesa[0] = 0;
	if (e.mao[e.ultimo_jogador] == 0)
		return e;
	if (e.inicio == 1 && jogador_inicial(e) != 0){
        e = jogam_bots (e);
		return e;
	}
	for(n=0;n<4;n++)
    	for(v = 0; v < 13; v++)
    		if(carta_existe(e.highlight, n, v)){
    			e.mao[0] = rem_carta (e.mao[0],n,v);
    			e.cartas_na_mesa[0] = add_carta(e.cartas_na_mesa[0], n,v);
			}
	e = verifica_conquistas(e);
	e.ultima_jogada = e.highlight;
	e.ultimo_jogador = 0;
	e.highlight = 0;
	e.inicio = 0;
	e = jogam_bots(e);
	return e;
}


/** \brief Irá indicar uma sugestão válida e maior que a jogada anterior (irá sugerir a menor carta caso não haja combinação anterior)
@param e Estado
@return Estado atualizado
*/

ESTADO sugestao(ESTADO e){
	if (e.ultimo_jogador == 0){
		e.ultima_jogada = 0;
		e.ultima_jogada = add_carta(e.ultima_jogada, 0, 0);
	}
	if (conta_cartas(e.ultima_jogada) == 1)
		e.highlight = uma_carta(e, 0);
    if (conta_cartas(e.ultima_jogada) == 2)
        e.highlight = duas_cartas(e, 0);
    if (conta_cartas(e.ultima_jogada) == 3)
        e.highlight = tres_cartas(e, 0);
	if (conta_cartas(e.ultima_jogada) == 5)
		e.highlight = cinco_cartas(e, 0);	
	return e;
}


/** \brief Função que permite que a mão do jogador não seja alterada, i.e. passar
@param e Estado atual
@return  Estado atualizado
*/

ESTADO passar(ESTADO e){
	if (e.mao[e.ultimo_jogador] == 0) return e;
	e = jogam_bots(e);
	e.cartas_na_mesa[0] = 0;
	e.passar = 0;
	return e;
}


/** \brief Função que permite que a seleção seja limpa.
@param e Estado atual
@return Estado atualizado
*/

ESTADO limpar(ESTADO e){
	e.highlight = 0;
	e.limpar = 0;
	return e;
}


/** \brief Irá colocar o parâmetro ordena a 1, se estiver a zero, e a 0 se estiver a 1
@param e Estado
@return Estado atualizado
*/

ESTADO ordena_cartas(ESTADO e){
	if (e.ordena_cartas == 0)
	   e.ordena_cartas = 1;
	else e.ordena_cartas = 0;
	e.ordena = 0;
	return e;
}


/** \brief Irá chamar a função sugestão
@param e Estado
@return Estado atualizado
*/

ESTADO sugere_sugestao(ESTADO e){
	e.highlight = 0;
	e = sugestao(e);
	e.sugestao = 0;
	return e;
}


/** \brief Irá determinar o que será executado a seguir (i.e. se é para jogar, passar ...)
Consoante o valor dos botões (ex: jogar = 1), irá chamar a função correspondente (neste caso chamaria a função jogar)
@param e Estado atual
@return Estado atualizado
*/

ESTADO proxima_acao(ESTADO e){
	if (e.jogar == 1)  e = jogar(e);
	if (e.passar == 1) e = passar(e);
	if (e.limpar == 1) e = limpar(e);
	if (e.ordena == 1) e = ordena_cartas(e);
	if (e.sugestao == 1) e = sugere_sugestao(e);
	if (e.proxima_ronda == 1){
		e = conta_pontuacao(e);
		if (acabou(e)){
			int vencedor = menor_pontuacao(e);
				char jogadores[4][6] = {"Sul", "Oeste", "Norte", "Este"};
				e.numero_jogo++;
				printf ("<text x=\"300\" y=\"500\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"100\" font-weight = \"bold\">%s venceu!</text>", jogadores[vencedor]);
			if (vencedor == 0) e.conquistas = add_carta(e.conquistas, 0, 8);
				e = reset_estado(0,0,0,0,e.numero_jogo,e.conquistas);

		}
	    e.proxima_ronda = 0;
	    e.numero_jogo++;
	    e = reset_estado(e.pontuacao[0], e.pontuacao[1], e.pontuacao[2], e.pontuacao[3], e.numero_jogo, e.conquistas);
		e = distribuir(e);
	}
	return e;
}


/** \brief Trata os argumentos da CGI

Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.

Caso a query esteja vazia, imprime o menu.
Caso no estado o parâmetro jogar seja 1, irá chamar a função jogar,
caso o parâmetro passar seja 1, irá chamar a função passar, etc.

Caso o jogo tenha acabado, irá fazer o reset_estado

@param query A query que é passada à cgi-bin
 */

void parse(char *query){
	if (query != NULL && strlen(query) != 0){
	    ESTADO e;
		e = str2estado(query);
		if (e.menu_instrucoes == 1){
			printf("<image x = \"0\" y = \"0\" height = \"1020\" width = \"1600\" xlink:href = \"%s/regras.svg\" /></a>\n",FILES);
			printf("<a xlink:href = \"http://127.0.0.1/cgi-bin/cartas\"><text fill = \"#ff0000\" x=\"1450\" y=\"1025\" text-anchor = \"left\" text-align = \"center\" font-family = \"Times New Roman\" font-size = \"35\" font-weight = \"\">VOLTAR</text>");
		}
		else{
			e = proxima_acao(e);
        	imprime(BARALHO, FILES, e);
        }
    }
	else{
		ESTADO e = {{0},{0},0,0,0,0,0,0,{0},0,0,0,0,0,0,0,0,0,0};
		e.numero_jogo = 1;
		e = distribuir(e);
		imprime_menu(FILES, e);
	}
}


/** \brief Função principal

Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas.
 */

int main() {
	imprime_janela_e_scripts();
/*
 * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING"));
	printf("</svg>\n");
	som();
	printf("</body>\n");
	return 0;
}