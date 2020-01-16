#include <stdlib.h>
#include <stdio.h>

/**
É equivalente a um long long int
*/
typedef long long int MAO;

/** 
@struct estado 
Estrutura que contem vários parâmetros para a execução do programa
*/
struct estado{
	MAO jogador; 		 ///< as cartas da mão do jogador inteligente
	MAO cartas_jogadas;  ///< todas as cartas que já foram jogadas
	MAO cartas_em_jogo;  ///< todas as cartas que ainda não foram jogadas, não contanto com as do jogador
	MAO ultima_jogada; 	 ///< a ultima jogada
	MAO jogada;			 ///< cartas que o jogador irá jogar
	int numero_cartas[4];///< número de cartas que cada um dos jogadores têm
	int inicio; 		 ///< indica se estamos no início do jogo
	int passou; 		 ///< indica quantos jogadores seguidos passaram
	int numero_jogador;  ///< indica o jogador atual
	int n_jogador;       ///< numero do jogador inteligente
};


/**
Define um tipo de struct estado chamado ESTADO
*/
typedef struct estado ESTADO;


/** \brief Devolve o índice da carta
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/

int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}


/** \brief Adiciona uma carta ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/

long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}


/** \brief Remove uma carta do estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/

long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}


/** \brief Verifica se uma carta pertence ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/

int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}


/** \brief Devolve o valor mais baixo de uma mão
@param m Mão
@return Valor mais baixo de uma mão
*/

int valor_mais_baixo(MAO m){
	int v,n;
	for (v=0;v<13;v++)
		for (n=0;n<4;n++)
			if (carta_existe(m, n, v))
				return v;
	return 0;
}


/** \brief Devolve o valor mais alto de uma mão
@param m Mão
@return Valor mais alto de uma mão
*/

int valor_mais_alto(MAO m){
	int v,n;
	for (v=12;v>=0;v--)
		for (n=0;n<4;n++)
			if (carta_existe(m, n, v))
				return v;
	return 0;
}

/** \brief Devolve o valor mais alto de uma mão
@param m Mão
@return Valor mais alto de uma mão
*/

int segundo_valor_mais_alto(MAO m){
	int v,n;
	for (v=12;v>=0;v--)
		for (n=0;n<4;n++)
			if (carta_existe(m, n, v))
				if (v != valor_mais_alto(m))
					return v;
	return 0;
}


/** \brief Conta quantas cartas tem uma mão
@param m Mão
@return  O número de cartas de uma mão
*/

int conta_cartas(MAO m){
	int n,v,conta=0;
	for (n=0;n<4;n++){
		for (v=0;v<13;v++){
			if (carta_existe(m, n, v)) 
				conta++;
		}
	}
	return conta;
}


/** \brief Conta quantas cartas de um determinado valor existem numa mão
@param m Mão
@param v Valor
@return Número de cartas de um determinado valor duma mão
*/
int conta_cartas_v(MAO m, int v){
	int n, c=0;
	for (n=0;n<4;n++){
		if (carta_existe(m, n, v))
           c++;
	}
    return c;
}


/** \brief Devolve o maior naipe de uma mão
@param m Mão
@return  Maior naipe de uma mão
*/

int naipe_mao(MAO m){
	int n, v, conta = conta_cartas(m), maior=0;
	while (conta > 0){
		for (n=0;n<4;n++){
			for (v=0;v<13;v++){
				if (carta_existe(m, n, v)){
					conta--;
					if (n > maior) maior = n;
				}
			}
		}
    }
    return maior;
}


/** \brief Indica se um determinado valor existe numa mão
@param m Mão
@param v Valor
@return 1 se determinado valor existe, 0 caso contrário
*/

int valor_existe(MAO m, int v){
    int n;
    for (n=0;n<4;n++)
    	if (carta_existe(m, n, v))
    		return 1;

    return 0;
}

/** \brief Indica qual é o valor mais baixo de um straight
Caso o 2 existir, 2 é o valor mais baixo, caso contrário começa a contar o ciclo a partir do 3
@param m Jogada
@return Valor mais baixo de um straight, -1 caso seja o 2, -2 caso seja A
*/

int valor_mais_baixo_straight(MAO m){
	if (valor_existe (m,11) && valor_existe(m,12)) return -2;
	if (valor_existe(m, 12)) return -1;
	else return (valor_mais_baixo (m));
}

/** \brief Irá remover as cartas de uma mão que se encontrão noutra
@param m1 Mão 1 (contem as cartas a ser removidas)
@param m2 Mão 2 (contem as cartas a remover)
@return m1 atualizada
*/

MAO remove_cartas(MAO m1, MAO m2){
	int v, n;
	for (v=0; v<13; v++)
		for (n=0; n<4; n++)
			if (carta_existe(m2, n, v) && carta_existe(m1, n, v))
				m1 = rem_carta(m1, n, v);
	return m1;
}

/** \brief Irá adicionar as cartas de uma mão a outra
@param m1 Mão 1 (onde as cartas serão adicionadas)
@param m2 Mão 2 (contem as cartas adicionar)
@return m1 atualizada
*/

MAO adiciona_cartas(MAO m1, MAO m2){
	int v, n;
	for (v=0; v<13; v++)
		for (n=0; n<4; n++)
			if (carta_existe(m2, n, v))
				m1 = add_carta(m1, n, v);
	return m1;
}


/** \brief Indica se uma determinada jogada é um straight
Primeiro irá procurar a combinação A-2-3-4-5, depois a 2-3-4-5-6 e depois as outras 
(visto que 2 é a carta mais baixa e A pode ser carta menor ou maior)
@param m Jogada
@return 1 caso se é um straight, 0 caso contrário
*/

int combinacao_straight(MAO m){
	int v, c, cf;
	v = valor_mais_baixo(m);
	cf = 0;
	if (valor_mais_alto(m) == 12) cf = 4;
	if (valor_existe(m, 11) && valor_existe(m, 12)) cf = 3;
	if (cf != 0){
		v = 0;
		for (c=0;c<cf;c++){
            if (valor_existe(m, v)) v++;
            else return 0;
		}
		return 1;
	}
	else for (c=0;c<5;c++){
			if (valor_existe(m, v) && v != 12) v++;
			else return 0;
		}
	return 1;

}


/** \brief Indica se uma determinada jogada é um flush, ou seja, verifica se todas as cartas são do mesmo naipe 
@param m Jogada
@return 1 se é um flush, 0 caso contrário
*/

int combinacao_flush(MAO m){
	int n, v, c=0;
	n = naipe_mao(m);
	v =  valor_mais_baixo(m);
	for (;v<13;v++){
		if (carta_existe(m, n, v))
			c++;
	}
	if (c==5) return 1;
	else return 0;

}


/** \brief Indica se uma determinada jogada é um fullhouse.
Encontra o par e o triplo (se de facto existirem) e irão verificar se o triplo tem 3 cartas e o par tem 2
@param m Jogada
@return 1 caso se é um fullhouse, 0 caso contrário
*/

int combinacao_fullhouse(MAO m){
	int n, v_min, v_max, num_min, num_max;
	v_min = valor_mais_baixo(m);
	v_max = valor_mais_alto(m);
	num_min = conta_cartas_v(m, v_min);
	num_max = conta_cartas_v(m, v_max);
	if (!((num_min == 3 && num_max ==2) || (num_min == 2 && num_max == 3)))
		return 0;
	for (n=0;n<4;n++){
		if (carta_existe(m, n, v_min))
			num_min--;
	}
	for (n=0;n<4;n++){
		if (carta_existe(m, n, v_max))
			num_max--;
	}
	if (num_min == 0 && num_max == 0)
		return 1;

	return 0;
}


/** \brief Indica se uma determinada jogada é um four of a kind
Verifica se existem 4 cartas do mesmo valor
@param m Jogada
@return 1 caso se é um four of a kind, 0 caso contrário
*/

int combinacao_fourofakind(MAO m){
	int n, v_min, v_max, num_min, num_max;
	v_min = valor_mais_baixo(m);
	v_max = valor_mais_alto(m);
	num_min = conta_cartas_v(m, v_min);
	num_max	= conta_cartas_v(m, v_max);
	if (!((num_min == 4 && num_max ==1) || (num_min == 1 && num_max == 4)))
		return 0;
	for (n=0;n<4;n++){
		if (carta_existe(m, n, v_min))
			num_min--;
	}
	for (n=0;n<4;n++){
		if (carta_existe(m, n, v_max))
			num_max--;
	}
	if (num_min == 0 && num_max == 0)
		return 1;
	return 0;
}


/** \brief Indica se uma determinada jogada é um straight flush.
Notar que primeiro irá procurar a combinação A-2-3-4-5, depois a 2-3-4-5-6 e depois as outras 
(visto que 2 é a carta mais baixa e A pode ser carta menor ou maior). Temos que procurar primeiro aqueles dois
casos devido à forma que as cartas (2 é o valor mais alto) estão ordenadas
@param m Jogada
@return 1 caso se é um straight flush, 0 caso contrário
*/

int combinacao_straightflush(MAO m){
	int v, n, c, cf;
	v = valor_mais_baixo(m); 
	n = naipe_mao(m);
	cf = 0;
	if (valor_mais_alto(m) == 12) cf = 4;
	if (valor_existe(m, 11) && valor_existe(m, 12)) cf = 3;
	if (cf != 0){
		v = 0;
		for (c=0;c<cf;c++){
            if (carta_existe(m, n, v)) v++;
            else return 0;
		}
		return 1;
	}
	else for (c=0;c<5;c++){
			if (carta_existe(m, n, v) && v != 12) v++;
			else return 0;
		 }
	return 1;
}


/** \brief Irá indicar qual é o tipo de combinação de uma jogada de 5 cartas
@param m Jogada
@return  1-Straight; 2-Flush; 3-Full House; 4-Four of a Kind; 5-Straight Flush.
*/
int tipo_combinacao(MAO m){
	if (combinacao_straightflush (m))  return 5;
	if (combinacao_straight      (m))  return 1;
	if (combinacao_flush         (m))  return 2;
	if (combinacao_fullhouse     (m))  return 3;
	if (combinacao_fourofakind   (m))  return 4;
	return 0;
}

/** \brief Indica qual é o valor que tem 3 ou 4 cartas do mesmo valor.
Esta função será útil para quando formos comparar uma jogada de 5 cartas
(fullhouse com fullhouse ou fourofakind com fourofakind) ver qual é a maior, 
visto que sabemos o valor do triplo ou quarto.
@param m Jogada
@return Valor do triplo ou quarto
*/

int valor_triplo_ou_quarto(MAO m){
	int v_min, v_max, num_min;
	v_min = valor_mais_baixo(m);
	v_max = valor_mais_alto(m);
	num_min = conta_cartas_v (m, v_min);
	if (num_min == 3 || num_min == 4) return v_min;
	else return v_max;
}


/** \brief Indica se 5 valores existem numa não (para usar em algumas combinações de 5 cartas)
@param m Mão
@param v1 Valor nº 1
@param v2 Valor nº 2
@param v3 Valor nº 3
@param v3 Valor nº 4
@param v3 Valor nº 5
@return 1 se e só se todos os valores existem, 0 caso contrário
*/

int valores_existem(MAO m, int v1, int v2, int v3, int v4, int v5){
	if (valor_existe (m,v1) && valor_existe (m,v2) && valor_existe (m,v3) &&
		valor_existe (m,v4) && valor_existe (m,v5))
		 return 1;
	else return 0;
}


/** \brief Devolve o naipe mais baixo de um determinado valor de uma mão
@param m Mão
@param v Valor
@return Naipe mais baixo do valor v
*/
int naipe_mais_baixo_v(MAO m, int v){
	int n;
	for (n=0;n<4;n++)
		if (carta_existe(m, n, v))
			return n;
	return 0;
}


/** \brief Devolve o maior naipe da maior carta de uma mão
@param m Mão
@return Naipe da maior carta
*/
int naipe_maior_carta(MAO m){
	int v, n;
	for (v=12;v>=0;v--){
		for (n=3;n>=0;n--){
			if (carta_existe (m, n, v))
				return n;
		}
	}
	return 0;
}

/** \brief Devolve o maior naipe da maior carta de uma mão straight
@param m Mão
@return Naipe da maior carta de um straight
*/

int naipe_maior_carta_straight(MAO m){
	int n;
	if (valor_existe(m,11) && valor_existe(m,12)){
		for (n=0;n<4;n++)
			if (carta_existe(m, n, 2)) return n;
	}
	else{ 
		if (valor_existe(m,12)){
			for (n=0;n<4;n++)
				if (carta_existe(m, n, 3)) return n;
		}
		else return naipe_maior_carta(m);
		}
	return 0;
}


/** \brief Indica se o jogador tem a maior carta em jogo
@param e Estado atual
@return 1 se verdadeiro, 0 se falso
*/

int maior_carta_jogo(ESTADO e){
	int n1 = naipe_maior_carta(e.cartas_em_jogo);
	int v1 = valor_mais_alto(e.cartas_em_jogo);
	int n2, v2;
	for (v2 = 12; v2 >= 0; v2--)
		for (n2 = 3; n2 >= 0; n2--)
			if (carta_existe(e.jogador, n2, v2)){
				if (v2 > v1 || (v2 == v1 && n2 > n1))
					return 1;
				else return 0;
			}
	return 0;
}

/** \brief Indica quantos valores diferentes existem numa mão
@param m Mão
@return Número de valores de uma mão
*/

int conta_valores(MAO m){
	int n,v, conta=0;
	for (v=0;v<13;v++)
		for (n=0;n<4;n++)
			if (carta_existe(m, n, v)){
				conta++;
				break;
			}
	return conta;
}


/** \brief Converte um char para um naipe (0,1,2 ou 3)
@param c 	Char que se vai converter
@return Naipe convertido para inteiro
*/

int char_para_naipe(char c){
	int n=0;
	switch(c){
		case 'D': n = 0; break;
		case 'C': n = 1; break;
		case 'H': n = 2; break;
		case 'S': n = 3; break;
	}
	return n;
}


/** \brief Converte um char para um valor (0,1,2,3,4,5,6,7,8,9,10,11 ou 12)
@param c 	Char que se vai converter
@return Valor convertido para inteiro
*/

int char_para_valor(char c){
	int v=0;
	if (c>='3' && c<='9') v = c - '3';
	switch(c){
		case 'T': v = 7;  break;
		case 'J': v = 8;  break;
		case 'Q': v = 9;  break;
		case 'K': v = 10; break;
		case 'A': v = 11; break;
		case '2': v = 12; break;
	}
	return v;
}


/** \brief Converte um valor para um char (3,4,5,6,7,8,9,T,J,Q,K,A,2)
@param v 	Valor que se vai converter
@return Valor convertido para char
*/

char valor_para_char(int v){
	int c=0;
	if (v >= 0 && v <= 6) c = v+51;
	switch(v){
		case 7:  c = 'T'; break;
		case 8:  c = 'J'; break;
		case 9:  c = 'Q'; break;
		case 10: c = 'K'; break;
		case 11: c = 'A'; break;
		case 12: c = '2'; break;
	}
	return c;
}


/** \brief Converte um naipe para um char (D,C,H ou S)
@param c 	Naipe que se vai converter
@return Naipe convertido para char
*/

int naipe_para_char(int n){
	char c=0;
	switch(n){
		case 0: c = 'D'; break;
		case 1: c = 'C'; break;
		case 2: c = 'H'; break;
		case 3: c = 'S'; break;
	}
	return c;
}


/** \brief Converte uma string para um tipo MAO (long long int)
@param s 	String que se vai converter
@return Mão
*/

MAO string_para_mao (char s[]){
	MAO m = 0;
	int i=0,v,n;
	while (s[i] != ' ') i++;
	for(i+=1;s[i-1]!='\0'; i+=3){
		v = char_para_valor(s[i]);
		n = char_para_naipe(s[i+1]);
		m = add_carta(m, n, v);
	}
	return m;
}



/** \brief Compara se duas strings são iguais
@param a 1ª string a comparar
@param b 2ª string a comparar
@return 1 se são iguais, 0 se são diferentes
*/

int compara (char a[], char b[]){
	int i;
	for (i=0;a[i] != '\0' && b[i] != '\0';i++)
		if (a[i] != b[i]) return 0;
	if (a[i] != '\0' || b[i] != '\0') return 0;
	else return 1;
}


/** \brief Imprime para a 'mesa' uma determinada jogada
Irá imprimir sob a forma de carateres.
Caso a jogada == 0, irá imprimir PASSOU.
@param m Mão a imprimir
*/

void print_jogada(MAO m){
	int n, v;
	char nc, vc;
	if (m == 0) printf("PASSO\n");
	else{
		for (v = 0; v < 13; v++)
			for (n = 0; n < 4; n++)
				if (carta_existe(m, n, v)){
					vc = valor_para_char(v);
					nc = naipe_para_char(n);
					printf("%c%c ", vc,nc);
				}
	printf("\n");
	}
}


/** \brief Irá dizer qual é o menor número de cartas dos outros jogadores
@param e Estado atual
@return número de cartas do jogador que tem menos cartas
*/

int menor_numero_cartas(ESTADO e){
	int i, c=13;
	for (i=0;i<4;i++)
		if (e.numero_cartas[i] < c && i != e.n_jogador){
			c = e.numero_cartas[i];
		}
	return c;
}


/** \brief Irá jogar a n cartas mais alta, i.e., jogar a 1 carta mais alta equivale a jogar a primeira carta mais alta.
Caso tente, por exemplo, jogar a 4 carta mais alta, mas só existir 3, irá jogar a terceira carta mais alta
@param e Estado atual
@param x Inteiro que irá determinar qual é a n carta mais alta
@return jogada com a carta mais alta
*/

MAO joga_n_carta_mais_alta(ESTADO e, int x){
	int v,n,c;
	int valor_ultima_jogada = valor_mais_alto(e.ultima_jogada);
	int naipe_ultima_jogada = naipe_mao(e.ultima_jogada);
	MAO jogada = 0;
	if (conta_cartas(e.jogador) < x)
		x = conta_cartas(e.jogador);
	for (c = x; c>=0; c--)
		for (v=12;v>=0;v--)
			for (n=4;n>=0;n--)
				if (carta_existe(e.jogador, n, v) && 
					(v > valor_ultima_jogada || (v == valor_ultima_jogada && n > naipe_ultima_jogada))){
					if (x == 1){
						jogada = add_carta(jogada, n, v);
						return jogada;
					}
					else x--;
				}
	return 0;
}


/** \brief Irá escolher a carta mais baixa (desde que só exista uma carta desse valor)
Caso contrário irá jogar a 4 carta mais alta
@param e Estado atual
@return Jogada com a carta mais baixa
*/

MAO uma_carta(ESTADO e){
	MAO jogada = 0;
	int n = 0, v = 0;
	if (e.ultima_jogada != 0 ){
		v = valor_mais_baixo(e.ultima_jogada);
		n = naipe_mao(e.ultima_jogada);
	}
	for (;v<13;v++){
		for (;n<4;n++)
			if (carta_existe(e.jogador, n, v) && conta_cartas_v(e.jogador, v) == 1){
				jogada = add_carta(jogada, n, v);
				return jogada;
			}
		n = 0;
	}
	jogada = joga_n_carta_mais_alta(e, 4);
	return jogada;
}


/** \brief Irá determinar qual é que vai ser a próxima carta jogada
@param e Estado atual
@return Jogada com a carta (ou vazia)
*/

MAO jogar_uma_carta(ESTADO e){
	MAO jogada = 0;
	int v_jogador = valor_mais_alto(e.jogador);
	int v2_jogador = segundo_valor_mais_alto(e.jogador);
	if (menor_numero_cartas(e) <= 2){
		jogada = joga_n_carta_mais_alta(e, 1);
		return jogada;
	}
	if (menor_numero_cartas(e) >= 3 && menor_numero_cartas(e) < 7 && conta_cartas(e.jogador) >= 4){
		if (maior_carta_jogo(e)){
			jogada = joga_n_carta_mais_alta(e, 1);
			return jogada;
		}
	}
	if (menor_numero_cartas(e) >=7 && conta_cartas(e.jogador) >= 7){
		jogada = uma_carta(e);
		if (conta_cartas_v(e.jogador, v_jogador) == 1 && ((valor_existe(jogada, v_jogador) || valor_existe(jogada, v2_jogador)) && (v_jogador >= 9 || v2_jogador >= 9)) && e.ultima_jogada != 0)
			jogada = 0;
	}
	else jogada = uma_carta(e);
	return jogada;
}


/** \brief Irá escolher o par mais baixo (desde que só existam duas cartas desse valor)
@param e Estado atual
@return Jogada com um o par mais baixo
*/

MAO par (ESTADO e){
	int n1,n2, v = 0;
	MAO jogada = 0;
	if (e.ultima_jogada)
		v = valor_mais_baixo(e.ultima_jogada);
	for (;v<13;v++)
		for (n1=0;n1<3;n1++)
			for (n2 = n1+1;n2<4;n2++)
				if (carta_existe(e.jogador, n1, v) && carta_existe(e.jogador, n2, v) 
				&& (conta_cartas_v(e.jogador, v) == 2 || menor_numero_cartas(e) <= 3) 
				&& (v > valor_mais_baixo(e.ultima_jogada) || n2 > naipe_mao(e.ultima_jogada))){
					jogada = add_carta((add_carta(jogada,n1,v)),n2,v);
					return jogada;
				}
	return jogada;
}

/** \brief Irá determinar qual é que vai ser o próximo par jogado.
@param e Estado atual
@return Jogada com um par (ou vazia)
*/

MAO jogar_par (ESTADO e){
	MAO jogada = 0;
	int v_jogador = valor_mais_alto(e.jogador);
	jogada = par(e);
	if (conta_cartas(e.jogador) >= 5 && menor_numero_cartas(e) >= 5 && valor_existe(jogada, v_jogador))
		jogada = 0;
	return jogada;
}


/** \brief Irá escolher o triplo mais baixo (desde que só existam três cartas desse valor)
@param e Estado atual
@return Jogada como triplo mais baixo
*/

MAO triplo (ESTADO e){
	int n1,n2,n3, v=0;
	MAO jogada = 0;
	if (e.ultima_jogada)
		v = valor_mais_baixo(e.ultima_jogada) +1;
	for (;v<13;v++)
		for (n1=0;n1<2;n1++)
			for (n2=n1+1;n2<3;n2++)
				for (n3=n2+1;n3<4;n3++)
					if (carta_existe(e.jogador, n1, v) && carta_existe(e.jogador, n2, v) && carta_existe(e.jogador, n3, v)){
						jogada = add_carta((add_carta((add_carta(jogada,n1,v)),n2,v)), n3, v);
						return jogada;
					}
	return jogada;
}


/** \brief Irá determinar qual é que vai ser o próximo triplo jogado.
@param e Estado atual
@return Jogada com um triplo (ou vazia)
*/

MAO jogar_triplo (ESTADO e){
	MAO jogada = 0;
	int v_jogador = valor_mais_alto(e.jogador);
	jogada = triplo(e);
	if (conta_cartas(e.jogador) >= 5 && menor_numero_cartas(e) >= 5 && valor_existe(jogada, v_jogador))
		jogada = 0;
	return jogada;
}


/** \brief Irá jogar um straight flush maior que o anterior, caso exista
@param e Estado
@return Jogada
*/

MAO straightflush (ESTADO e){
	int v,n,v1,v2,v3,v4,v5;
	MAO jogada = 0;
	n = 0; v = 0;
	if (tipo_combinacao(e.ultima_jogada) == 5){
			n = naipe_mao(e.ultima_jogada)+1;
			v = valor_mais_baixo_straight(e.ultima_jogada);
	}
	for (v1 = v, v2 = v+1, v3 = v+2, v4 = v+3, v5 = v+4; v<8; v++, v1++,v2++,v3++,v4++,v5++){
    	for(;n<4;n++){
    		if (carta_existe(e.jogador, n, v1) && carta_existe(e.jogador, n, v2) && carta_existe(e.jogador, n, v3) 
    			&& carta_existe(e.jogador, n, v4) && carta_existe(e.jogador, n, v5)){
   				jogada = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (jogada, n, v1)), n, v2)), n,v3)), n,v4)), n,v5);
   				if (e.inicio && !(carta_existe(jogada,0,0)))
					return 0;
				return jogada;
			}
    	}
  		n = 0;
  	}
	return 0;
}


/** \brief Irá jogar um four of a kind maior que a combinação anterior, caso exista
Caso não exista, irá tentar jogar um straight flush
@param e Estado
@return Jogada
*/

MAO fourofakind (ESTADO e){
	int vq,v,n;
	MAO jogada = 0;
	if (tipo_combinacao(e.ultima_jogada) == 4)
		vq = valor_triplo_ou_quarto(e.ultima_jogada) +1;
	else vq = 0;
	for (;vq<13;vq++)
		if (carta_existe(e.jogador, 0, vq) && carta_existe(e.jogador, 1,vq) && carta_existe(e.jogador, 2, vq) && carta_existe(e.jogador, 3, vq))
			for (v=0;v<13;v++){
				if (v == vq) v++;
				for (n=0;n<4;n++)
					if (carta_existe(e.jogador, n, v)){
						jogada = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (jogada, 0, vq)), 1, vq)), 2,vq)), 3,vq)), n,v);
						if (e.inicio && !(carta_existe(jogada,0,0)))
							return straightflush(e);
						return jogada;
					}
			}
	return straightflush(e);
}


/** \brief Irá jogar um full house maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um four of a kind
@param e Estado
@return Jogada
*/

MAO fullhouse (ESTADO e){
	int vt,nt1,nt2,nt3,vp,np1,np2; 
	MAO jogada = 0;
	if (tipo_combinacao (e.ultima_jogada) == 3)
		vt = valor_triplo_ou_quarto(e.ultima_jogada) +1;
	else vt = 0;
	for (;vt<13;vt++)
		for (nt1=0;nt1<2;nt1++)
			for (nt2=nt1+1;nt2<3;nt2++)
				for (nt3=nt2+1;nt3<4;nt3++)
					if (carta_existe(e.jogador, nt1, vt) && carta_existe(e.jogador, nt2,vt) && carta_existe(e.jogador, nt3, vt))
						for (vp=0;vp<13;vp++){
							if (vp == vt) vp++;
							for (np1=0;np1<3;np1++)
								for (np2=np1+1;np2<4;np2++)
									if (carta_existe(e.jogador, np1, vp) && carta_existe(e.jogador, np2,vp)){
										jogada = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (jogada, nt1, vt)), nt2, vt)), nt3,vt)), np1,vp)), np2,vp);
										if (e.inicio && !(carta_existe(jogada,0,0)))
												return fourofakind(e);
										return jogada;
									}
							if (vp == 12) return 0;
						}
	return fourofakind(e);
}


/** \brief Irá jogar um flush maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um full house
@param e Estado
@return Jogada
*/

MAO flush (ESTADO e){
	int n,v,c, valores[5] = {0};
	MAO jogada = 0;
	if (tipo_combinacao(e.ultima_jogada) == 2)
		n = naipe_mao(e.ultima_jogada);
	else n = 0;
	for (c=0;n<4 && c<5;n++){
		c = 0;
		for (v=0;v<13 && c<5;v++)
			if (carta_existe(e.jogador, n, v))
				if(c!=4 || n > naipe_mao(e.ultima_jogada) || v > valor_mais_alto(e.ultima_jogada)){
						valores[c] = v;
						c++;
				}
	}
	if (c == 5){
		n--;
		jogada = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (jogada, n, valores[0] )), n, valores[1])), n,valores[2])), n,valores[3])), n,valores[4]);
			if (e.inicio && !(carta_existe(jogada,0,0)))
				return fullhouse(e);
		return jogada;
	}
	return fullhouse(e);
}


/** \brief Irá jogar um straight maior que a combinação anterior, caso exista
Caso não existir, irá tentar jogar um flush
@param e Estado
@return Jogada
*/

MAO straight (ESTADO e){
	int v=0, v1,v2,v3,v4,v5,n5;
	MAO jogada = 0;
	if (tipo_combinacao(e.ultima_jogada) == 1)
		v = valor_mais_baixo_straight(e.ultima_jogada);
	for(v1 = v, v2 = v+1, v3 = v+2, v4 = v+3, v5 = v+4; v<8; v++,v1++,v2++,v3++,v4++,v5++){
		if (v == valor_mais_baixo_straight(e.ultima_jogada) && !(e.inicio))
			n5 = naipe_maior_carta_straight(e.ultima_jogada)+1;
		else n5 = naipe_mais_baixo_v(e.jogador, v5);
		for (;n5<4; n5++)
			if (valores_existem(e.jogador, v1, v2, v3, v4, v5) && carta_existe(e.jogador, n5, v5)){
				int n1 = naipe_mais_baixo_v(e.jogador, v1);
				int n2 = naipe_mais_baixo_v(e.jogador, v2);
				int n3 = naipe_mais_baixo_v(e.jogador, v3);
				int n4 = naipe_mais_baixo_v(e.jogador, v4);
				jogada = add_carta ((add_carta ((add_carta ((add_carta ((add_carta (jogada, n1, v1)), n2, v2)), n3,v3)), n4,v4)), n5,v5);
				if (e.inicio && !(carta_existe(jogada,0,0))){
					return flush(e);
				}
	    		return jogada;
    		}
    }
    return flush(e);
}


/** \brief Joga uma jogada quando estamos no início do jogo
@param e Estado atual
@return Jogada inicial (0 se não tem o 3D)
*/

ESTADO joga_inicio(ESTADO e){
	int cartas_v = conta_cartas_v(e.jogador, 0);
	if (!carta_existe(e.jogador,0,0))
		e.jogada = 0;
	else{
		if (cartas_v == 4)
			e.jogada = fourofakind(e);
		if (cartas_v == 3 || cartas_v == 2){
			e.jogada = fullhouse(e);
			if (!e.jogada && cartas_v == 3) e.jogada = jogar_triplo(e);
			if (!e.jogada) e.jogada = jogar_par(e);
		}
		if (cartas_v == 1 || !e.jogada){
			e.jogada = straight(e);
			if (!e.jogada) {
				e.jogada = jogar_uma_carta(e);
			}
		}
	}
	return e;
}


/** \brief Função auxiliar para a função jogar_0.
Irá tentar jogar uma combinação de 5 cartas, caso mão conseguir tenta jogar 3, etc...
@param e Estado atual
@return Jogada
*/

MAO jogar_0_aux(ESTADO e){
	int v = valor_mais_alto(e.jogador);
	MAO jogada = 0;
	jogada = fullhouse(e);
	if (conta_cartas_v(e.jogador, v) < conta_cartas_v(jogada, v))
		jogada = 0;
	if (!jogada) jogada = straight(e);
	if (conta_cartas_v(e.jogador, v) < conta_cartas_v(jogada, v) && menor_numero_cartas(e) > 5){
		e.ultima_jogada = jogada;
		jogada = 0;
	}
	if (!jogada) jogada = flush(e);
	if (conta_cartas_v(e.jogador, v) < conta_cartas_v(jogada, v) && menor_numero_cartas(e) > 5){
		e.ultima_jogada = 0;
		jogada = 0;
	}
	if (!jogada) jogada = jogar_triplo(e);
	if (!jogada || (conta_cartas_v(jogada,v) == 3 && menor_numero_cartas(e) > 3 ))
		jogada = jogar_par(e);
	if (!jogada || (conta_cartas_v(jogada,v) == 2 && menor_numero_cartas(e) > 2 ))
		jogada = jogar_uma_carta(e);
	return jogada;
}


/** \brief Irá jogar uma combinação quando o parametro e.ultima_jogada = 0, ou seja, os outros 3 jogadores passaram
@param e Estado atual
@return Estado atualizado
*/

ESTADO jogar_0(ESTADO e){
	int numero_cartas = conta_cartas(e.jogador);
	int numero_valores = conta_valores(e.jogador);
	if (numero_cartas >= 5){
		e.jogada = jogar_0_aux(e);
	}
	if ((numero_cartas == 3 || numero_cartas == 4) && numero_valores == 2 && maior_carta_jogo(e)){
		e.jogada = jogar_triplo(e);
		if (!e.jogada) e.jogada = jogar_par(e);
		return e;
	}
	if (numero_cartas == 4 || numero_cartas == 3){
		if (numero_valores == 1 || (numero_cartas == 4 && numero_valores == 2))
			e.jogada = jogar_triplo(e);
		if ((numero_valores == 2 && numero_cartas == 3) || ((numero_valores == 3 && numero_cartas == 4) || (numero_valores == 2 && numero_cartas == 4 && !e.jogada)))
			e.jogada = jogar_par(e);
		if (numero_valores == 4 || (numero_valores == 3 && numero_cartas == 3))
			e.jogada = jogar_uma_carta(e);
	}
	if (numero_cartas == 2){
		if (numero_valores == 1)
			e.jogada = jogar_par(e);
		else if (maior_carta_jogo(e)) e.jogada = joga_n_carta_mais_alta(e, 1);
			 else e.jogada = jogar_uma_carta(e);
	}
	if (numero_cartas == 1 || !e.jogada)
		e.jogada = jogar_uma_carta(e);

	return e;
}


/** \brief Irá jogar uma combinação quando o parametro e.ultima_jogada != 0, ou seja, pelo menos 1 dos ultimos 3 jogadores jogou
@param e Estado atual
@return Estado atualizado
*/

ESTADO jogar_1 (ESTADO e){
	int numero_cartas = conta_cartas(e.ultima_jogada);
	if (numero_cartas == 5){
		int tipo = tipo_combinacao(e.ultima_jogada);
		if (tipo == 5) e.jogada = straightflush(e);
		if (tipo == 4) e.jogada = fourofakind(e);
		if (tipo == 3) e.jogada = fullhouse(e);
		if (tipo == 2) e.jogada = flush(e);
		if (tipo == 1) e.jogada = straight(e);
	}
	if (numero_cartas == 3) e.jogada = jogar_triplo(e);
	if (numero_cartas == 2) e.jogada = jogar_par(e);
	if (numero_cartas == 1) e.jogada = jogar_uma_carta(e);
	return e;
}


/** Irá chamar as funções adequadas conforme o estado do jogo (por exemplo, se os outros 3 passaram, irá chamar jogar_0)
@param e Estado atual
@return Estado atualizado
*/

ESTADO jogar (ESTADO e){
	if (e.passou == 3)
		e.ultima_jogada = 0;
	e.passou = 0;
	if (e.inicio){ 
		e = joga_inicio(e);
		e.inicio = 0;
	}
	else{
		if (!e.ultima_jogada)
			 e = jogar_0(e);
		else e = jogar_1(e);

	}
	e.jogador -= e.jogada;
	print_jogada(e.jogada);
	e.jogada = 0;
	return e;
}


/** \brief Irá receber o input e irá reagir de forma adequada
@param e Estado atual
@param input Input recebido
@return Estado atualizado
*/

ESTADO parse (ESTADO e, char *input){
	e.jogada = 0;
	if (compara(input,"OK") || compara(input,"NAO")) return e;
	if (compara(input,"PASSOU")){
		e.numero_jogador++;
		if (e.numero_jogador == 4) e.numero_jogador = 0;
		e.passou++;
		return e;
	}
	if (compara(input,"JOGADA")){
		e.numero_jogador++;
		if (e.numero_jogador == 4) e.numero_jogador = 0;
		e.n_jogador = e.numero_jogador;
		e = jogar(e);
	}
	else{
		e.numero_jogador++;
		if (e.numero_jogador == 4) e.numero_jogador = 0;
		e.inicio = 0;
		e.passou = 0;
		e.ultima_jogada = string_para_mao(input);
		e.cartas_jogadas = adiciona_cartas (e.cartas_jogadas, e.ultima_jogada);
		e.numero_cartas[e.numero_jogador] -= conta_cartas(e.ultima_jogada);
		e.cartas_em_jogo = remove_cartas (e.cartas_em_jogo, e.cartas_jogadas);
	}
	return e;
}



/** \brief Irá encarregar de receber o primeiro input e colocar as cartas na mão do jogador e continuará a receber inputs até receber "ACABOU"
@return 0;
*/
int main (){
	ESTADO e = {0,0,0,0,0,{13,13,13,13},1,0,-1,0};
	char mao_jogador[43];
	char input[21];
	if (scanf("%[^\n]",mao_jogador) != 0)
		e.jogador = string_para_mao(mao_jogador);
	e.cartas_em_jogo = 4503599627370495 - e.jogador;
	getchar();
	if (scanf("%[^\n]", input) != 0){
		while (!compara(input,"ACABOU")){
			e = parse(e, input);
			getchar();
			if (scanf("%[^\n]", input) != 0) {};
		}
	}
	return 0;
}