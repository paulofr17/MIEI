/**
É equivalente a um long long int
*/
typedef long long int MAO;

/**
@struct estado 
Estrutura que contem as informações para o jogo de cartas, desde as mãos de cada jogador, o valor dos botões, as pontuações etc...
*/
struct estado {
  MAO mao[4];            /* array que contem as mãos de cada jogador (i.e. um long long int) */
  MAO cartas_na_mesa[4]; /* array que contem as cartas de cada jogador que estão na mesa  */
  MAO highlight;         /* ontem as cartas selecionadas pelo jogador  */
  int jogar;             /* controla se é para jogar (1) ou não(0)  */
  int passar;            /* controla se o jogador passou (1) ou não (0)  */
  int inicio;            /* indica se é (1) ou não (0) o início do jogo  */
  int limpar;            /* indica se o jogador limpou a jogada (1) ou não (0)  */
  int proxima_ronda;     /* controla se o jogador carregou no botão para a próxima ronda  */
  int pontuacao[4];      /* array com a pontuação de cada jogador  */
  MAO ultima_jogada;     /* contem a ultima mão jogada  */
  int ultimo_jogador;    /* contem o número do ultimo jogador  */
  int proximo_jogo;      /* indica se chegamos ao próximo jogo  */
  int numero_jogo;       /* indica o número do jogo (ronda) atual  */
  int ordena;            /* indica se o jogador carregou no botão para ordenar as cartas  */
  int ordena_cartas;     /* indica por que ordem se irão ordenar as cartas (0 por valor, 1 por naipe) */ 
  int sugestao;          /* indica se o jogador carregou no botão sugestão  */
  MAO conquistas;        /* long long int que contem as conquistas do jogador (as conquistas são guardadas sobre formas de cartas)  */ 
  int menu_jogar;        /* indica se estamos (1) ou não (0) no menu  */
  int menu_instrucoes;   /* indica se o jogador carregou no botão de instruções  */
};

/**
Formato para as funções de conversão de estado para string e vice-versa
*/
#define FORMATO	"%lld_%lld_%lld_%lld_%lld_%d_%d_%lld_%d_%d_%d_%d_%lld_%lld_%lld_%lld_%d_%d_%d_%d_%d_%d_%d_%d_%lld_%d_%d"

/**
Define um tipo de struct estado chamado ESTADO
*/
typedef struct estado ESTADO;

/**
Converte ESTADO para string
*/
char* estado2str(ESTADO e){
  static char str[10240];
  sprintf(str, FORMATO,
  	e.mao[0], e.mao[1], e.mao[2], e.mao[3], 
  	e.highlight,
  	e.jogar, e.passar, e.ultima_jogada, e.ultimo_jogador, e.inicio, e.limpar, e.proxima_ronda,
  	e.cartas_na_mesa[0], e.cartas_na_mesa[1], e.cartas_na_mesa[2], e.cartas_na_mesa[3], 
  	e.pontuacao[0], e.pontuacao[1], e.pontuacao[2], e.pontuacao[3],
    e.numero_jogo,
    e.ordena, e.ordena_cartas,
    e.sugestao,
    e.conquistas,
    e.menu_jogar, e.menu_instrucoes);

  return str;
}


/**
Converte string para ESTADO
*/
ESTADO str2estado(char* str){
  ESTADO e;
  sscanf(str, FORMATO, 
  	&e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], 
  	&e.highlight,
  	&e.jogar, &e.passar, &e.ultima_jogada, &e.ultimo_jogador, &e.inicio, &e.limpar, &e.proxima_ronda,
  	&e.cartas_na_mesa[0], &e.cartas_na_mesa[1], &e.cartas_na_mesa[2], &e.cartas_na_mesa[3], 
  	&e.pontuacao[0], &e.pontuacao[1], &e.pontuacao[2], &e.pontuacao[3],
    &e.numero_jogo,
    &e.ordena, &e.ordena_cartas,
    &e.sugestao,
    &e.conquistas,
    &e.menu_jogar, &e.menu_instrucoes);

  return e;
}


/** \brief Irá limpar o estado para ser usado numa próxima ronda
Para isso irá colocar todos os parâmetros a zero à exceção da pontuação,
das conquistas e do número da ronda atual
@param a      Pontuação do jogador 0
@param b      Pontuação do jogador 1
@param c      Pontuação do jogador 2
@param d      Pontuação do jogador 3
@param nj       Número do jogo (ronda) atual
@param conquistas   Conquistas atuais
@return Estado atualizado
*/

ESTADO reset_estado(int a, int b, int c, int d, int nj, long long int conquistas){
   ESTADO e = {{0},{0},0,0,0,0,0,0,{0},0,0,0,0,0,0,0,0,0,0};
   e.pontuacao[0] = a;
   e.pontuacao[1] = b;
   e.pontuacao[2] = c;
   e.pontuacao[3] = d;
   e.numero_jogo = nj;
   e.conquistas = conquistas;
   return e;
}


/** \brief Irá remover as cartas que estão na mesa
@param e Estado atual
@return Estado atualizado
*/

ESTADO limpa_cartas_mesa(ESTADO e){
  e.cartas_na_mesa[0] = 0;
  e.cartas_na_mesa[1] = 0;
  e.cartas_na_mesa[2] = 0;
  e.cartas_na_mesa[3] = 0;
  return e;
}

#include "cartas_e_jogadas.h"
/**
Irá devolver um numero consoante o número de cartas que cada jogador tem.
Se tem <= 10, devolve um ponto por carta;
Se tem 11 e 12, devolve dois pontos por carta;
Se tem 13, devolve 3 pontos por carta.
*/
#define SCORE(x) ((x > 10) ? ((x) == 13 ? 39 : 2 * (x)) : (x))

/** \brief Conta a pontuação de cada jogador, ou seja, o número de cartas aplicado ao 'SCORE'.
@param e Estado
@return Estado atualizado
*/

ESTADO conta_pontuacao(ESTADO e){
  e.pontuacao[0] += SCORE(conta_cartas (e.mao[0]));
  e.pontuacao[1] += SCORE(conta_cartas (e.mao[1]));
  e.pontuacao[2] += SCORE(conta_cartas (e.mao[2]));
  e.pontuacao[3] += SCORE(conta_cartas (e.mao[3]));
  return e;
}
