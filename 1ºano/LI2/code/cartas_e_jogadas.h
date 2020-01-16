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


/** \brief Devolve o valor de uma mão
@param m Mão
@return  -1 se tem mais que um valor, caso contrário devolve o valor dessa mão
*/

int valor_mao(MAO m){
	int n, v, i=0,conta, valor[13];
	conta = conta_cartas(m);
	while (i != conta){
		for (n=0;n<4;n++){
			for (v=0;v<13;v++){
				if (carta_existe(m, n ,v)){
					valor[i] = v; i++;
				}
			}
	     }
    }
	for (i=0;i<conta-1;i++){
		if (valor[i] != valor[i+1]) return -1;
	}
	return valor[0];
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


/** \brief Diz se uma combinação é válida.
Para verificar se uma combinação é valida, irá verificar se tem pelo menos uma carta.
Se tiver entre uma e 3 cartas (inclusive) irá verificar se tem todas o mesmo valor
(ou seja, se é uma única carta ou um par ou triplo).
Caso tenha 5 cartas, irá verificar se é algum tipo de combinação de 5 cartas.
@param m Jogada
@return  1 se Verdadeiro, 0 se Falso
*/

int combinacao_valida(MAO m){
	if (conta_cartas(m) > 0 && conta_cartas(m) < 4 && valor_mao(m) != -1)
		return 1;
	else if (conta_cartas(m) == 5){
			if (combinacao_straightflush (m))	return 1;
			if (combinacao_straight      (m))	return 1;
			if (combinacao_flush         (m))	return 1;
			if (combinacao_fullhouse     (m))	return 1;
			if (combinacao_fourofakind   (m))	return 1;
		}
	return 0;
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


/** \brief Indica qual é o valor mais alto de um straight 
Não podemos usar a função normal visto que 2 é a carta menor e A pode ser a carta menor ou maior.
Caso exista um A e 2, estamos na presença de um A-2-3-4-5 (carta maior tem valor 2).
Notar que se não existir A, o ciclo começa a contar a partir de A (visto que 2 é o menor valor)
@param m Jogada
@return Valor mais alto de um straight
*/

int valor_mais_alto_straight(MAO m){
	int v;
	if (valor_existe(m, 11) && valor_existe(m ,12)) return 2;
	for (v=11;v>=0;v--){
		if (valor_existe(m, v))
			return v;
	}
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


/** \brief Compara duas combinações de cinco cartas
Irá comparar a partir dos tipos de combinação, ou seja, se um tipo de combinação
for maior que outro, quer dizer que a combinação é maior. Caso sejam do mesmo tipo
irá verificar (segundo as regras) qual é o maior. Ou seja, caso sejam os dois
um  straight, irá verificar quem tem a carta maior.
@param m1 Mão nº1
@param m2 Mão nº2
@return 1 se mão 1 > mão 2, 0 caso contrário
*/

int combinacao_maior_cinco(MAO m1, MAO m2){
  int t1 = tipo_combinacao(m1);				int t2 = tipo_combinacao(m2);
  int vs1 = valor_mais_alto_straight(m1);	int vs2 = valor_mais_alto_straight(m2);
  int v1 = valor_mais_alto(m1);				int v2 = valor_mais_alto(m2);
  int n1 = naipe_maior_carta(m1);			int n2 = naipe_maior_carta(m2);
  
  if (t1 > t2) return 1;

  if (t1 < t2) return 0;

  if (t1 == 5)
  	if (vs1 > vs2 || (vs1 == vs2 && n1 > n2)) return 1;

  if (t1 == 4 || t1 == 3)
  	if (valor_triplo_ou_quarto(m1) > valor_triplo_ou_quarto(m2)) return 1;

  if (t1 == 2)
  	if (n1 > n2 || (n1 == n2 && v1 > v2)) return 1;

  if (t1 == 1)
  	if (vs1 > vs2 || (vs1 == vs2 && n1 > n2)) return 1;

  return 0;
}


/** \brief Diz se uma combinação é maior que outra
@param m1 Mão nº 1
@param m2 Mão nº 2
@return  1 se mão 1 > mão 2, 0 caso contrário
*/

int combinacao_maior(MAO m1, MAO m2){
	if (conta_cartas(m1) != conta_cartas(m2))
		return 0;
	if (conta_cartas(m1) == 5){
		if (combinacao_maior_cinco(m1,m2)) return 1;
		else return 0;
	}
	if (valor_mao(m1) > valor_mao(m2))
		return 1;
	if (valor_mao(m1) == valor_mao(m2) && naipe_mao(m1) > naipe_mao (m2))
		return 1;
	return 0;
}


/** \brief Diz quem tem o 3 de ouros
@param m Mão
@return  Número do jogar que tem o 3 de ouros.
*/

int jogador_inicial(ESTADO e){
	int m;
	for (m=0;m<4;m++)
		if (carta_existe(e.mao[m], 0, 0))
			return m;
	return 0;
}

/** \brief Indica se estamos no início de um jogo, ou seja, se todos os jogadores tem 13 cartas
@param e Estado
@return 1 se Verdadeiro, 0 se Falso
*/

int inicio(ESTADO e){
	int m;
	for (m=0;m<4;m++){
		if (carta_existe(e.mao[m], 0, 0)) return 1;
	}
	return 0;
}


/** \brief Indica se um jogo acabou, i.e., se alguém tem 100 pontos ou mais
@param e Estado
@return 1 se o jogo acabou, 0 caso contrário
*/

int acabou(ESTADO e){
	int i;
	for (i=0;i<4;i++)
		if (e.pontuacao[i] >= 100)
			return 1;
	return 0;
}


/** \brief Indica qual é o jogador com menos pontuação (para determinar o vencedor quando o jogo terminar)
@param e Estado
@return Jogador com menor pontuação
*/

int menor_pontuacao(ESTADO e){
	int i, min=0;
	for (i=1;i<4;i++)
		if (e.pontuacao[min] > e.pontuacao[i])
			min = i;
	return min;
}

/** \brief Remove as cartas de uma mão que existem noutra
@param m1 Mão onde se irá remover as cartas
@param m2 Mão que contém as cartas a remover
@return Mão com as cartas removidas
*/

MAO remove_cartas(MAO m1, MAO m2){
	int n, v;
		for (n = 0; n < 4; n++)
	for (v = 0; v < 13; v++)
			if (carta_existe(m2, n, v))
				m1 = rem_carta(m1, n, v);
	return m1;
}

/** \brief Diz se é possível jogar, i.e. se a combinações do utilizador é válida e maior que a anterior
@param e Estado
@return  1 se Verdadeiro, 0 se Falso
*/

int posso_jogar(ESTADO e){
	if (!combinacao_valida(e.highlight))
		return 0;
	if (e.inicio && !carta_existe(e.highlight,0,0)) return 0;
	if(e.ultimo_jogador == 0)
		return 1;
	if(combinacao_maior(e.highlight, e.ultima_jogada) && conta_cartas(e.mao[e.ultimo_jogador]) != 0)
	  return 1;
    return 0;
}