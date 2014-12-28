:- use_module(library(lists)). /*reverse*/

par(Numero):- Temp is Numero rem 2, Temp =:= 0. /*verifica se numero é par*/
impar(Numero):- Temp is (Numero-1) rem 2, Temp =:= 0. /*verifica se numero é impar*/

/*////////CRIAR TABULEIRO VAZIO//////////*/

celulaVazia(X,Y,[X,Y,'vazia','vazia']).

novoTabuleiro(Tamanho, Tabuleiro):- 
	
	/*parte de cima*/ /*gera desde a linha central, até ao topo do tabuleiro*/
	LinhasQueFaltam is Tamanho//2+1,
	TamanhoLinha is Tamanho,
	X is -(Tamanho//2),
	novoTabuleiroCima(LinhasQueFaltam, TamanhoLinha, X, 0, ParteCimaT),
	reverse(ParteCimaT,ParteCima),

	/*parte de baixo*/ /*gera a parte do tabuleiro abaixo da linha central*/
	LinhasQueFaltam1 is Tamanho//2,
	TamanhoLinha1 is Tamanho-1,
	X1 is -(Tamanho//2),
	novoTabuleiroBaixo(LinhasQueFaltam1, TamanhoLinha1, X1, 1, ParteBaixo),
	append(ParteCima,ParteBaixo,TabuleiroTemp),

	Temp  is -Tamanho//2, colocaPeca([Temp, 0, 'preta' ,'simples'],TabuleiroTemp , T1), 
	Temp1 is  Tamanho//2, colocaPeca([Temp1, 0, 'branca','simples'],T1, Tabuleiro),!.


novaLinha(0,_,_,[]).
novaLinha(CelulasQueFaltam, X, Y, [Cabeca | Resto]):-
	celulaVazia(X,Y,Cabeca),
	Cnovo is CelulasQueFaltam-1,
	Xnovo is X+1,
	novaLinha(Cnovo, Xnovo, Y, Resto). 


novoTabuleiroCima(0, _, _, _, []).
novoTabuleiroCima(LinhasQueFaltam, TamanhoLinha, Xincial, Y, [Cabeca | Resto]):-
	/*write('a construir linha: '),write(Y),nl,*/
	novaLinha(TamanhoLinha,Xincial,Y,Cabeca),
	Lnovo is LinhasQueFaltam-1,
	Tnovo is TamanhoLinha-1,
	Xnovo is Xincial+1,
	Ynovo is Y-1,
	novoTabuleiroCima(Lnovo, Tnovo, Xnovo, Ynovo,Resto).


novoTabuleiroBaixo(0, _, _, _, []).
novoTabuleiroBaixo(LinhasQueFaltam, TamanhoLinha, Xinicial, Y, [Cabeca | Resto]):-
 /*write('a construir linha: '),write(Y),nl,*/
 novaLinha(TamanhoLinha,Xinicial,Y,Cabeca),
 Lnovo is LinhasQueFaltam-1,
 Tnovo is TamanhoLinha-1,
 Ynovo is Y+1,
 novoTabuleiroBaixo(Lnovo, Tnovo, Xinicial, Ynovo,Resto).




/*/////////////////////////////////////////*/



/*//////////IMPRIMIR TABULEIRO////////////*/

simboloCelula('vazia', 'vazia', '   O  ').
simboloCelula('preta', 'simples', '  -X- ').
simboloCelula('preta', 'ataque', '  -A- ').
simboloCelula('preta', 'defesa', '  -D- ').
simboloCelula('preta', 'salto', '  -S- ').
simboloCelula('preta', 'expansao', '  -E- ').
simboloCelula('branca', 'simples', '   X  ').
simboloCelula('branca', 'ataque', '   A  ').
simboloCelula('branca', 'defesa', '   D  ').
simboloCelula('branca', 'salto', '   S  ').
simboloCelula('branca', 'expansao', '   E  ').
/*simboloCelula(_, _, '  ? ').*/
simboloCelula([_,_,Cor,Tipo], Simbolo):- simboloCelula(Cor,Tipo, Simbolo).

converteXRealTabuleiro(X, _, Xtabuleiro):- Xtabuleiro is X//2 -1.
converteXTabuleiroReal(X, Y, XReal):- par(Y), XReal is (X+1)*2.
converteXTabuleiroReal(X, Y, XReal):- impar(Y), XReal is (X+3/2)*2.



imprimeTabuleiro(Tabuleiro):- length(Tabuleiro, Tamanho), 
	limparEcra,
	write('||||||||||||||||||| TABULA |||||||||||||||||||'),nl,
	T is Tamanho*2 - 1, write('    '), imprimeLinhaNumeros(T, 0), 
	imprimeTabuleiro1(Tabuleiro),
	write('    '), imprimeLinhaNumeros(T, 0), nl.


imprimeLinhaNumeros(Fim,Fim):-nl,!.
imprimeLinhaNumeros(Fim, I):-( (I < 10, write('  ')); I>=10, write(' ')), write(I), Inovo is I+1, imprimeLinhaNumeros(Fim,Inovo).


imprimeTabuleiro1([]).
imprimeTabuleiro1([Cabeca | Resto]):- imprimeLinhaInit(Cabeca), imprimeTabuleiro1(Resto).
imprimeLinha([]):-nl,nl.
imprimeLinha([Celula | Resto]):- imprimeCelula(Celula), imprimeLinha(Resto).
imprimeLinhaInit( [ [X, R, Cor, Tipo] | Resto ] ):- (R<0; R>=0,write(' ')), write(R),
	Offset is (abs(R)+(1/2)), imprimeOffset(Offset), imprimeLinha( [ [X, R, Cor, Tipo] | Resto ] ).
imprimeCelula([_, _, Cor, Tipo]):- simboloCelula(Cor, Tipo, Simbolo), write(Simbolo)/*, write(X), write(' | '), write(Y)*/.



imprimeOffset(X):- X <  (1/2), write('Erro imprimir offset').
imprimeOffset(X):- X =:= (1/2), write(' ').
imprimeOffset(X):- X >  (1/2), write('   '), X1 is X-1, imprimeOffset(X1).




limparEcra:-nl/*,nl,nl,nl,nl,nl,nl,nl,nl/*,nl,nl,nl,nl,nl,
	nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,
	nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,
	nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,
	nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl*/.
/*/////////////////////////////////////////*/


/*//////FUNCOES MANIPULACAO TABULEIRO//////*/

celulaVazia([_,_,'vazia','vazia']).
pecaPreta([_,_,'preta',_]).
pecaBranca([_,_,'branca',_]).
pecaSimples([_,_,_,'simples']).
pecaAtaque([_,_,_,'ataque']).
pecaDefesa([_,_,_,'defesa']).
pecaSalto([_,_,_,'salto']).
pecaExpansao([_,_,_,'expansao']).

tipoValido('ataque').
tipoValido('defesa').
tipoValido('simples').
tipoValido('expansao').
tipoValido('salto').


/* COLOCAR PECA: usar colocaPeca(Peca, TabuleiroVelho, TabuleiroNovo). */
colocaPecaEmCelula([X, Y, Cor, Tipo], [X, Y, Cor, Tipo]).
colocaPecaEmCelula(X, Y, Cor, Tipo, [X, Y, Cor, Tipo]).


colocaPecaLinha(_, [], [], Sucesso, Sucesso).









/* coloca peça em remodelação - versão anterior comentada em cima */

colocaPecaLinha(_,[],[]).

colocaPecaLinha([X, Y, Cor, Tipo], [[X, Y, Cor1, Tipo1] | RestoVelho],[CabecaNovo | RestoNovo]):-
	celulaVazia([X,Y,Cor1,Tipo1]), /*write('estava vazia'), nl,*/ colocaPecaEmCelula(X,Y,Cor,Tipo,CabecaNovo), 
	colocaPecaLinha([X, Y, Cor, Tipo], RestoVelho, RestoNovo).

colocaPecaLinha([X, Y, Cor, Tipo], [[X1, Y1, Cor1, Tipo1] | RestoVelho],[CabecaNovo | RestoNovo]):-
	/*(X1 \= X; Y1 \= Y),*/
	colocaPecaEmCelula(X1, Y1, Cor1, Tipo1 ,CabecaNovo),
	colocaPecaLinha([X, Y, Cor, Tipo], RestoVelho, RestoNovo).


colocaPeca(_,[],[]).
colocaPeca(Peca, [LinhaVelho | RestoVelho], [LinhaNovo | RestoNovo]):-
	colocaPecaLinha(Peca,LinhaVelho,LinhaNovo),
	colocaPeca(Peca,RestoVelho,RestoNovo)/*,!*/.





/* offsets da coordenadas das celulas vizinhas de uma determinada peça */
coordVizinhos([[+1,  0], [+1, -1], [ 0, -1], [-1,  0], [-1, +1], [ 0, +1]]).


/*verificar se existem pecas da cor da peça dada, nas células suas vizinhas*/ 
/* USAR ESTA: mesmaCorNaVizinhanca(Celula, Tabuleiro). */
/* mesmaCorNaVizinhancaCelula(Celula, Outra Celula, Deltas dos Vizinhos (coordVizinhos(Deltas))) */
mesmaCorNaVizinhancaCelula( _ , _ , []):-1==2.
mesmaCorNaVizinhancaCelula([X,Y,Cor,_], [X1, Y1, Cor1, _], [ [DeltaX , DeltaY] | RestoVizinhos]):-
	/*write('X='),write(X),write(', Y='), write(Y), write(', X1='), write(X1),write(', Y1='), write(Y1), write(', deltaX='), write(DeltaX),write(', deltaY='),write(DeltaY),nl,*/
	( X =:= X1+DeltaX, Y =:= Y1+DeltaY, Cor == Cor1, ! ; mesmaCorNaVizinhancaCelula([X,Y,Cor,_], [X1, Y1, Cor1, _], RestoVizinhos) ).
mesmaCorNaVizinhanca(_, []):-1==2.
mesmaCorNaVizinhanca(Celula, [Linha | Resto]):- (mesmaCorNaVizinhancaLinha(Celula, Linha), !; mesmaCorNaVizinhanca(Celula, Resto)).
mesmaCorNaVizinhancaLinha(_, []):- 1==2.
mesmaCorNaVizinhancaLinha(Celula, [ Celula1 | _]):- coordVizinhos(V), mesmaCorNaVizinhancaCelula(Celula,Celula1,V).
mesmaCorNaVizinhancaLinha(Celula, [ _ | RestoVizinhos]):- mesmaCorNaVizinhancaLinha(Celula, RestoVizinhos).




/* uma peça está protegida, se estiver adjacente a uma peça de defesa da mesma cor, ou se for de defesa */
/* chamada: pecaProtegida(Peca, Tabuleiro). */
pecaProtegidaCelula( _ , _ , []):-1==2.
pecaProtegidaCelula([X,Y,Cor,_], [X1, Y1, Cor1, Tipo1], [ [DeltaX , DeltaY] | RestoVizinhos]):-
	( Tipo1 == 'defesa', X =:= X1+DeltaX, Y =:= Y1+DeltaY, Cor == Cor1, ! ; pecaProtegidaCelula([X,Y,Cor,_], [X1, Y1, Cor1, Tipo1], RestoVizinhos) ).
pecaProtegida(_, []):-1==2.
pecaProtegida([_,_,_,'defesa'],_).
pecaProtegida(Celula, [Linha | Resto]):- (pecaProtegidaLinha(Celula, Linha), !; pecaProtegida(Celula, Resto)).
pecaProtegidaLinha(_, []):- 1==2.
pecaProtegidaLinha(Celula, [ Celula1 | _]):- coordVizinhos(V), pecaProtegidaCelula(Celula,Celula1,V).
pecaProtegidaLinha(Celula, [ _ | RestoVizinhos]):- pecaProtegidaLinha(Celula, RestoVizinhos).




/* Eliminar peças de cor oposta, à volta de uma determinada célula,
	excepto se forem peças de defesa, ou peças protegidas (adjacentes a defesa)*/
/* chamada: eliminaCelulaVolta(Celula, TabuleiroVelho, TabuleiroNovo) */
eliminaCelulaVolta(_, [],[], _, Removidas, Removidas).
eliminaCelulaVolta(Celula, [LinhaVelho | RestoVelho], [LinhaNovo | RestoNovo], TabuleiroAux, AuxRemovidas, PecasRemovidas):- 
	eliminaCelulaVoltaLinha(Celula, LinhaVelho, LinhaNovo, TabuleiroAux, [], RemovidasTemp),
	append(RemovidasTemp, AuxRemovidas, RemovidasTotal),

	eliminaCelulaVolta(Celula, RestoVelho, RestoNovo, TabuleiroAux, RemovidasTotal, PecasRemovidas).


eliminaCelulaVoltaLinha(_, [], [], _, Removidas, Removidas).
eliminaCelulaVoltaLinha(Celula, [CelulaVelho | RestoVelho], [CelulaNovo | RestoNovo], TabuleiroAux, AuxRemovidas, PecasRemovidas):-
	coordVizinhos(Vizinhos), eliminaCelulaVoltaCelula(Celula, CelulaVelho, CelulaNovo, Vizinhos, TabuleiroAux, [], RemovidasTemp),
	append(RemovidasTemp, AuxRemovidas, RemovidasTotal),
	eliminaCelulaVoltaLinha(Celula, RestoVelho, RestoNovo, TabuleiroAux, RemovidasTotal, PecasRemovidas).

eliminaCelulaVoltaCelula( _, CelulaVelha, CelulaNova, [], _, _, []):- colocaPecaEmCelula(CelulaVelha, CelulaNova).
eliminaCelulaVoltaCelula([X,Y,Cor,Tipo], [X1, Y1, Cor1, Tipo1], CelulaNova, [ [DeltaX , DeltaY] | RestoVizinhos], TabuleiroAux, AuxRemovidas, PecasRemovidas):-
	Xtemp is X+DeltaX, Ytemp is Y+DeltaY,
	( X1 =:= Xtemp, Y1 =:= Ytemp, Cor \== Cor1, /*write(Cor),write(Cor1),nl,*/Tipo1\=='defesa', 
		\+pecaProtegida([X1, Y1, Cor1, Tipo1], TabuleiroAux), Tipo1\=='vazia', /*write('Erase'),nl,*/
	celulaVazia(X1, Y1, CelulaNova), 
		append(AuxRemovidas, [[X1, Y1, Cor1, Tipo1]], PecasRemovidas), !;
	eliminaCelulaVoltaCelula([X,Y, Cor,Tipo], [X1, Y1, Cor1, Tipo1], CelulaNova, RestoVizinhos, TabuleiroAux, AuxRemovidas, PecasRemovidas) ).



/* para ser usado com as peças de expansao */
adicionaPecaVolta([X,Y,Cor,'expansao'], TabuleiroV,TabuleiroN, PecaAdicionada):- 
	adicionaPecaVolta([X,Y,Cor,'expansao'], TabuleiroV,TabuleiroN, TabuleiroV, 'nao', _, PecaAdicionada).

adicionaPecaVolta(_, [],[], _, Sucesso,Sucesso, PecaAdicionada).
adicionaPecaVolta(Celula, [LinhaVelho | RestoVelho], [LinhaNovo | RestoNovo], TabuleiroAux, Sucesso, SucessoN, PecaAdicionada):- 
	adicionaPecaVoltaLinha(Celula, LinhaVelho, LinhaNovo, TabuleiroAux, Sucesso, SucessoTemp, PecaAdicionada),
	adicionaPecaVolta(Celula, RestoVelho, RestoNovo, TabuleiroAux, SucessoTemp, SucessoN, PecaAdicionada).


adicionaPecaVoltaLinha(_, [], [], _, Sucesso, Sucesso, PecaAdicionada).
adicionaPecaVoltaLinha(Celula, [CelulaVelho | RestoVelho], [CelulaNovo | RestoNovo], TabuleiroAux, Sucesso, SucessoN, PecaAdicionada):-
	coordVizinhos(Vizinhos), adicionaPecaVoltaCelula(Celula, CelulaVelho, CelulaNovo, Vizinhos, TabuleiroAux, Sucesso, SucessoTemp, PecaAdicionada),
	adicionaPecaVoltaLinha(Celula, RestoVelho, RestoNovo, TabuleiroAux, SucessoTemp, SucessoN, PecaAdicionada).

adicionaPecaVoltaCelula( _, CelulaVelha, CelulaNova, [], _, Sucesso, Sucesso, PecaAdicionada):- 
	colocaPecaEmCelula(CelulaVelha, CelulaNova).

adicionaPecaVoltaCelula([X,Y,Cor,Tipo], [X1, Y1, Cor1, Tipo1], CelulaNova, [ [DeltaX , DeltaY] | RestoVizinhos], TabuleiroAux, Sucesso, SucessoN, PecaAdicionada):-
	(Sucesso == 'sim', SucessoN = 'sim', colocaPecaEmCelula([X1, Y1, Cor1, Tipo1], CelulaNova);

	Xtemp is X+DeltaX, Ytemp is Y+DeltaY,
	( X1 =:= Xtemp, Y1 =:= Ytemp, Tipo1=='vazia',/* write('Adiciona'),nl,*/
	colocaPecaEmCelula(X1, Y1, Cor, 'simples', CelulaNova), SucessoN = 'sim', PecaAdicionada = CelulaNova, !;
	adicionaPecaVoltaCelula([X,Y, Cor,Tipo], [X1, Y1, Cor1, Tipo1], CelulaNova, RestoVizinhos, TabuleiroAux, Sucesso, SucessoN, PecaAdicionada) ) ).




/*chamada: isCelulaVazia(X, Y, Tabuleiro). */
isCelulaVazia(_,_,[]):- 1==2, write(celulaOcupada).
isCelulaVazia(X, Y, [Linha | Resto]):- (isCelulaVaziaLinha(X, Y, Linha); isCelulaVazia(X,Y,Resto)), !.

isCelulaVaziaLinha(_,_,[]):- 1==2.
isCelulaVaziaLinha(X, Y, [[X, Y, 'vazia', 'vazia'] | _]):- !.
isCelulaVaziaLinha(X, Y, [[X1, Y1, _, _] | Resto]):- (X \== X1 ; Y \== Y1), isCelulaVaziaLinha(X,Y,Resto).



/*/////////////////////////////////////////*/


copiaTabuleiro(Tabuleiro,Tabuleiro).



/*/////////////CICLO JOGO/////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/* pede jogada, e coloca peça. Se primeira peça for simples,
	pede então uma segunda peça, simples também */
	pedirJogada1(Jogador,Tamanho, TabuleiroV, TabuleiroN):-
	write('Jogador: '), write(Jogador), nl, 
	pedirJogada(Jogador, [X, Y, Cor, Tipo],Tamanho),
	(
		colocaPeca( [X, Y, Cor, Tipo], TabuleiroV, TabuleiroTemp),
		

			(Tipo=='salto',copiaTabuleiro(TabuleiroTemp,TabuleiroN);
	
			\+(mesmaCorNaVizinhanca([X, Y, Cor, Tipo], TabuleiroV)),
			write('Sem vizinhos da mesma cor...'), nl, pedirJogada1(Jogador, Tamanho, TabuleiroV, TabuleiroN);

			Tipo == 'simples', pedirJogada2(Jogador, Tamanho, TabuleiroTemp, TabuleiroN); 

			Tipo == 'ataque', accaoAtaque([X,Y,Cor,'ataque'], TabuleiroTemp, TabuleiroN);

			copiaTabuleiro(TabuleiroTemp, TabuleiroN))

		;

		write('Celula ocupada...'), nl, pedirJogada1(Jogador, Tamanho, TabuleiroV, TabuleiroN)
	)
	
	.

/* usada para pedir uma segunda peça para ser jogada (simples) */
pedirJogada2(Jogador,Tamanho, TabuleiroV, TabuleiroN):-
	imprimeTabuleiro(TabuleiroV),
	write('Jogador: '), write(Jogador), nl, 
	write('Pode jogar mais uma peca simples!'),nl,
	pedirJogada(Jogador, [X, Y, Cor, _],Tamanho),
	(colocaPeca([X, Y, Cor, 'simples'],  TabuleiroV, TabuleiroTemp),
		(\+(mesmaCorNaVizinhanca([X, Y, Cor, 'simples'], TabuleiroV)),write('Sem vizinhos da mesma cor...'), nl;
		copiaTabuleiro(TabuleiroTemp,TabuleiroN));
	write('Celula ocupada...'), nl, pedirJogada2(Jogador, Tamanho, TabuleiroV, TabuleiroN)).


jogar(Tamanho):- novoTabuleiro(Tamanho, T),
	imprimeTabuleiro(T), jogar('preta', T, Tamanho).
	
	
jogar(Jogador, Tabuleiro, Tamanho):- 
	
	pedirJogada1(Jogador, Tamanho, Tabuleiro, TNovo1),
	processaPecasEspeciais(TNovo1, TNovo, PecasAdicionadas, PecasRemovidas),
	\+fimDoJogo(TNovo),
	
	imprimeTabuleiro(TNovo), !, 
	jogadorSeguinte(Jogador, JNovo),
	jogar(JNovo, TNovo, Tamanho).

	
	/*Homem vs Maquina*/
jogar1(Tamanho):-novoTabuleiro(Tamanho, T),colocaPeca([Temp, 0, 'preta' ,'simples'],T , T1),
	Temp  is -Tamanho//2, colocaPeca([Temp, 0, 'preta' ,'simples'],T , T1), 
	Temp1 is  Tamanho//2, colocaPeca([Temp1, 0, 'branca','simples'],T1, T2),
	write('Com que peças quer jogar: preta ou branca: '),read(Jogador),jogar1(Jogador, T2, Tamanho).

jogar1(Jogador,Tabuleiro,Tamanho):-
	pedirJogada1(Jogador, Tamanho, Tabuleiro, TNovo),
	processaPecasEspeciais(TNovo, TNovo1, PecasAdicionadas1, PecasRemovidas1),
	\+fimDoJogo(TNovo1),
	imprimeTabuleiro(TNovo1),
	jogadorSeguinte(Jogador, JNovo),
	fazJogadaComputador(JNovo,TNovo1,TNovo2, _),
	processaPecasEspeciais(TNovo2, TNovo3, PecasAdicionadas2, PecasRemovidas2),
	\+fimDoJogo(TNovo3),
	imprimeTabuleiro(TNovo3),
	jogar1(Jogador, TNovo3, Tamanho).
	
	/* Maquina vs Maquina*/
jogar2(Tamanho):-novoTabuleiro(Tamanho, T),colocaPeca([Temp, 0, 'preta' ,'simples'],T , T1),
	Temp  is -Tamanho//2, colocaPeca([Temp, 0, 'preta' ,'simples'],T , T1), 
	Temp1 is  Tamanho//2, colocaPeca([Temp1, 0, 'branca','simples'],T1, T2),
	imprimeTabuleiro(T2),jogar2('preta', T2, Tamanho),write('Acabou o jogo').


jogar2(Jogador,Tabuleiro,Tamanho):-
	fazJogadaComputador(Jogador,Tabuleiro,TNovo1, _),
	processaPecasEspeciais(TNovo1, TNovo, PecasAdicionadas1, PecasRemovidas1),
	imprimeTabuleiro(TNovo), !,
	(fimDoJogo(TNovo);

	(jogadorSeguinte(Jogador, JNovo),
	fazJogadaComputador(JNovo,TNovo,TNovo2, _),
	processaPecasEspeciais(TNovo2, TNovo3, PecasAdicionadas2, PecasRemovidas2),
	imprimeTabuleiro(TNovo3), !, 
		(fimDoJogo(TNovo3);
		jogar2(Jogador, TNovo3, Tamanho)))).
	


/* vamos chamar os jogadores pelas cores da peças que jogam */
jogadorSeguinte('preta','branca').
jogadorSeguinte('branca','preta').

escrevePeca([X, Y, Jogador, Tipo]):-
write('X:'),write(X),
write(' Y:'),write(Y),
write(' Jogador: '),write(Jogador),
write(' Tipo: '),write(Tipo),nl.



pedirJogada(Jogador, [X, Y, Jogador, Tipo],Tamanho):-
	write('   X?'), read(X1),
	write('   Y?'), read(YTemp),
	T is round(Tamanho//2),
	(
	((YTemp<0;YTemp=0),
	PrimeiroX is -T-YTemp,						/*valor x da primeira celula na grelha*/
	OndeComeca is -YTemp,						/*valor x da primeira celula no tabuleiro grafico*/
	XN is (X1-OndeComeca)/2+PrimeiroX)		/*valor x traduzido*/
	
	;
	
	(YTemp>0,
	PrimeiroX is -T,
	OndeComeca is YTemp,
	XN is (X1-OndeComeca)/2+PrimeiroX)
	
	
	),
	/*write('PrimeiroX:'),write(PrimeiroX),nl,
	write('OndeComeca:'),write(OndeComeca),nl,
	write('Xnovo:'),write(XN),nl,*/
	
	write('Tipo?'), read(TipoTemp),
	(tipoValido(TipoTemp), X is round(XN), Y is YTemp, Tipo = TipoTemp; 
		write('Tipo invalido...'),nl,pedirJogada(Jogador, [X, Y, Jogador, Tipo],Tamanho)).


/*chama a accao que cada tipo de peca tem, O Tnovo será onde estara o novo tabuleiro apos as accoes das pecas*/
accaoPeca([X, Y, Jogador, 'ataque'],Tabuleiro,TNovo):-accaoAtaque([X, Y, Jogador, 'ataque'],Tabuleiro,TNovo).
accaoPeca([_, _, _, 'defesa'],Tabuleiro,Tabuleiro).
accaoPeca([_, _, _, 'expansao'],Tabuleiro,Tabuleiro).
accaoPeca([_, _, _, 'salto'],Tabuleiro,Tabuleiro).

/*se for de ataque elimina todas as ceclulas do outro jogador, menos as de defesa*/
accaoAtaque(Peca,Tabuleiro,Tnovo, AuxRemovidas, PecasRemovidas):- 
	/*write('vai eliminar?'), nl,*/ eliminaCelulaVolta(Peca,Tabuleiro,Tnovo, Tabuleiro, AuxRemovidas, PecasRemovidas).



	
fimDoJogo(Tabuleiro):- fimDoJogo(Tabuleiro, 0, 0, P, B),
	( (P>B, write('GANHA O PRETO'), nl); (B>P, write('GANHA O BRANCO'), nl); (P =:= B, write('EMPATE')) ).

fimDoJogo([], P, B, P, B):- write('Pretas: '), write(P), write(' | Brancas: '), write(B), nl.
fimDoJogo([Linha | Resto] , Paux, Baux, P, B):- fimDoJogoLinha(Linha, Paux, Baux, P1, B1),
	fimDoJogo(Resto, P1, B1, P, B).

fimDoJogoLinha( [[_, _, 'vazia', _] | _], _, _, _, _):-  1 == 2. /*se houver uma vazia, o jogo nao acabou*/
fimDoJogoLinha( [[_, _, 'preta', _] | Resto], Paux, Baux, P, B):- Pnovo is Paux + 1,
	fimDoJogoLinha(Resto, Pnovo, Baux, P, B).
fimDoJogoLinha( [[_, _, 'branca', _] | Resto], Paux, Baux, P, B):- Bnovo is Baux + 1,
	fimDoJogoLinha(Resto, Paux, Bnovo, P, B).
fimDoJogoLinha( [], Paux, Baux, Paux, Baux).



/*//////////////////////////////////////////////////////////////////*/
/* FUNCOES PARA PERCORRER O TABULEIRO E APLICAR O EFEITO DAS PEÇAS  */
/*//////////////////////////////////////////////////////////////////*/

/* chamada processaPecasEspeciais(TabuleiroVelho, TabuleiroNovo)*/
processaPecasEspeciais(TabuleiroVelho, TabuleiroNovo, PecasAdicionadas, PecasRemovidas):- 
	processaPecasEspeciais(TabuleiroVelho, TabuleiroVelho, TabuleiroNovo, [],[], PecasAdicionadas, PecasRemovidas).

processaPecasEspeciais([],Tabuleiro, Tabuleiro, PecasAdicionadas, PecasRemovidas, PecasAdicionadas, PecasRemovidas).
processaPecasEspeciais([LinhaVelho | RestoVelho], TabuleiroAux, TabuleiroFinal, AuxAdicionadas, AuxRemovidas, PecasAdicionadas, PecasRemovidas):-
	/*write('processaPecasEspeciais'),nl,*/
	processaPecasEspeciaisLinha(LinhaVelho, TabuleiroAux, TabuleiroTemp, [], [], AdicionadasTemp, RemovidasTemp),

	append(AdicionadasTemp, AuxAdicionadas, AdicionadasTotal), append(RemovidasTemp, AuxRemovidas, RemovidasTotal),

	processaPecasEspeciais(RestoVelho, TabuleiroTemp, TabuleiroFinal, AdicionadasTotal, RemovidasTotal, PecasAdicionadas, PecasRemovidas).


processaPecasEspeciaisLinha([],Tabuleiro,Tabuleiro, Adicionadas, Removidas, Adicionadas, Removidas).
processaPecasEspeciaisLinha([CelulaVelho | RestoVelho], TabuleiroAux, TabuleiroF, AuxAdicionadas, AuxRemovidas, PecasAdicionadas, PecasRemovidas):-

	((celulaVazia(CelulaVelho);pecaSimples(CelulaVelho)), 
		processaPecasEspeciaisLinha(RestoVelho, TabuleiroAux, TabuleiroF, AuxAdicionadas, AuxRemovidas, PecasAdicionadas, PecasRemovidas);

	processaPecasEspeciaisCelula(CelulaVelho, TabuleiroAux, TabuleiroTemp, [], [], AdicionadasTemp, RemovidasTemp), 

	append(AdicionadasTemp, AuxAdicionadas, AdicionadasTotal), append(RemovidasTemp, AuxRemovidas, RemovidasTotal),

	processaPecasEspeciaisLinha(RestoVelho, TabuleiroTemp, TabuleiroF, AdicionadasTotal, RemovidasTotal, PecasAdicionadas, PecasRemovidas)).


processaPecasEspeciaisCelula([_,_,_,'vazia'], Tabuleiro,Tabuleiro, [],[],[],[]).
processaPecasEspeciaisCelula([_,_,_,'simples'], Tabuleiro,Tabuleiro, [],[],[],[]).
processaPecasEspeciaisCelula([_,_,_,'defesa'], Tabuleiro,Tabuleiro, [],[],[],[]).
processaPecasEspeciaisCelula([_,_,_,'salto'], Tabuleiro,Tabuleiro, [],[],[],[]).

processaPecasEspeciaisCelula([X,Y,Cor,'ataque'], TabuleiroV,TabuleiroN, AuxAdicionadas, AuxRemovidas, PecasAdicionadas, PecasRemovidas):-
	accaoAtaque([X,Y,Cor,'ataque'], TabuleiroV, TabuleiroN, AuxRemovidas, PecasRemovidas).

processaPecasEspeciaisCelula([X,Y,Cor,'expansao'], TabuleiroV,TabuleiroN, AuxAdicionadas, AuxRemovidas, PecasAdicionadas, PecasRemovidas):-
	adicionaPecaVolta([X,Y,Cor,'expansao'], TabuleiroV,TabuleiroN, PecaAdicionada),
	( (is_list(PecaAdicionada), PecasAdicionadas = [PecaAdicionada] ) ; PecasAdicionadas = []).
	
	
	
/* ///////////////////////// A I ////////////////////////// */
	
imprimeLista([[X,Y]|Resto]):-write(X),write('|'),write(Y),nl,imprimeLista(Resto).
imprimeLista([]):-!.
	
/*Encontra todas as posicoes Livres*/
encontraPosicoesLivres(Lista,[],Lista).
encontraPosicoesLivres(Lista,[Cabeca|Resto],NovaLista):-encontraPosicoesLivresLinha(Lista,Cabeca,NovaListaTemp),
														encontraPosicoesLivres(NovaListaTemp,Resto,NovaLista).

encontraPosicoesLivresLinha(Lista,[],Lista).
encontraPosicoesLivresLinha(Lista,[Celula|Resto],NovaLista):-encontraPosicoesLivresCelula(Lista,Celula,LL)
															,encontraPosicoesLivresLinha(LL,Resto,NovaLista).

encontraPosicoesLivresCelula(Lista,[X,Y,Cor,Tipo],NovaLista):- Cor == 'vazia', Tipo=='vazia', add([X,Y],Lista,NovaLista);copiaTabuleiro(Lista,NovaLista).


add(X,L,[X|L]).







geraListaJogadasValidas([], _, [], _):-!.
geraListaJogadasValidas([ [X, Y] | RestoPosicoes], Jogador, [Jogada | RestoJogadas], Tabuleiro):-
	colocaPecaEmCelula([X,Y,Jogador,'salto'],Jogada),
	geraListaJogadasValidasOutras([ [X, Y] | RestoPosicoes], Jogador, RestoJogadas, Tabuleiro).
geraListaJogadasValidasOutras([ [X, Y] | RestoPosicoes], Jogador, [Jogada1, Jogada2, Jogada3, Jogada4 | RestoJogadas], Tabuleiro):-
	isCelulaVazia(X,Y,Tabuleiro), mesmaCorNaVizinhanca([X, Y, Jogador, _], Tabuleiro),
	colocaPecaEmCelula([X,Y,Jogador,'ataque'],Jogada1),
	colocaPecaEmCelula([X,Y,Jogador,'defesa'],Jogada2),
	colocaPecaEmCelula([X,Y,Jogador,'expansao'],Jogada3),
	colocaPecaEmCelula([X,Y,Jogador,'simples'],Jogada4),
	geraListaJogadasValidas(RestoPosicoes, Jogador, RestoJogadas, Tabuleiro).
geraListaJogadasValidasOutras([ _ | RestoPosicoes], Jogador, Jogadas, Tabuleiro):-
	geraListaJogadasValidas(RestoPosicoes, Jogador, Jogadas, Tabuleiro).


imprimeJogadas([]):-!.
imprimeJogadas([[X,Y,Cor,Tipo]|Resto]):-write('X: '), write(X),write(', Y: '),write(Y),
	write(', Cor: '),write(Cor), write(', Tipo: '), write(Tipo),nl,
	imprimeJogadas(Resto).

imprimeInfoCelula([X,Y,Cor,Tipo]):-write(X),write('|'),write(Y),write(' = '),write(Cor),write('|'),write(Tipo),nl.

/* daqui para baixo nao testei */

fazJogadaComputador(Jogador, TabuleiroVelho, TabuleiroNovo, JogadasFeitas):- 
	encontraPosicoesLivres([],TabuleiroVelho,Livres),
	geraListaJogadasValidas(Livres, Jogador, Jogadas, TabuleiroVelho),
	chooseRandom(Jogadas, Jogada1),
	
	JogadasFeitasTemp = [Jogada1],

	write('Jogador :'),write(Jogador),nl,
	imprimeInfoCelula(Jogada1),
	colocaPeca(Jogada1,TabuleiroVelho, TabuleiroTemp),
	(pecaSimples(Jogada1), fazJogadaComputador2(Jogador,TabuleiroTemp,TabuleiroNovo, JogadaFeita), 
		append(JogadasFeitasTemp, [JogadaFeita], JogadasFeitas);

		copiaTabuleiro(TabuleiroTemp,TabuleiroNovo), JogadasFeitas = JogadasFeitasTemp).

fazJogadaComputador2(Jogador, TabuleiroVelho, TabuleiroNovo, JogadaFeita):-
	encontraPosicoesLivres([],TabuleiroVelho,Livres),
	geraListaJogadasValidas(Livres, Jogador, Jogadas, TabuleiroVelho),
	escolheRandomJogadaSimples(Jogadas,Jogada),

	(Jogada == [], copiaTabuleiro(TabuleiroVelho, TabuleiroNovo);
		colocaPeca(Jogada,TabuleiroVelho, TabuleiroNovo), JogadaFeita = Jogada).


escolheRandomJogadaSimples([],[]).
escolheRandomJogadaSimples([Jogada | Resto], JogadaF):-
	chooseRandom([Jogada | Resto], JogadaTemp),
	(pecaSimples(JogadaTemp), colocaPecaEmCelula(JogadaTemp,JogadaF);
		escolheRandomJogadaSimples(Resto,JogadaF)).

:- use_module(library(random)).

%% choose(List, Elt) - chooses a random element
%% in List and unifies it with Elt.
chooseRandom([], []).
chooseRandom(List, Elt) :-
        length(List, Length),
        random(0, Length, Index),
        nth0(Index, List, Elt).
		
limpaEcra:-nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl.
escreverCabecalho(Opcao,Tamanho):-limpaEcra,
write('**************  TABULA  **************'),nl,
write('*                                    *'),nl,
write('*    1.Jogar Humano-Humano           *'),nl,
write('*    2.Jogar Humano-Computador       *'),nl,
write('*    3.Jogar Computador-Computador   *'),nl,
write('*                                    *'),nl,
write('**************************************'),nl,
write('Opcao:'),read(Opcao),
write('Tamanho:'),read(Tamanho),limpaEcra.
		
tabula:-escreverCabecalho(Opcao,Tamanho),(
Opcao==1,jogar(Tamanho);
Opcao==2,jogar1(Tamanho);
Opcao==3,jogar2(Tamanho);
tabula).




tabuleiroExemplo1(
[[[0,-4,vazia,vazia],[1,-4,'preta','expansao'],[2,-4,'preta','simples'],[3,-4,'vazia','vazia'],[4,-4,'vazia','vazia']],
[[-1,-3,'vazia','vazia'],[0,-3,'vazia','vazia'],[1,-3,'vazia','vazia'],[2,-3,'vazia','vazia'],[3,-3,'vazia','vazia'],[4,-3,'vazia','vazia']],
[[-2,-2,'vazia','vazia'],[-1,-2,'vazia','vazia'],[0,-2,'vazia','vazia'],[1,-2,'vazia','vazia'],[2,-2,'vazia','vazia'],[3,-2,'vazia','vazia'],[4,-2,'vazia','vazia']],
[[-3,-1,'vazia','vazia'],[-2,-1,'vazia','vazia'],[-1,-1,'vazia','vazia'],[0,-1,'vazia','vazia'],[1,-1,'vazia','vazia'],[2,-1,'vazia','vazia'],[3,-1,'vazia','vazia'],[4,-1,'vazia','vazia']],
[[-4,0,'preta','simples'],[-3,0,'vazia','vazia'],[-2,0,'vazia','vazia'],[-1,0,'vazia','vazia'],[0,0,'vazia','vazia'],[1,0,'vazia','vazia'],[2,0,'vazia','vazia'],[3,0,'vazia','vazia'],[4,0,'branca','simples']],
[[-4,1,'vazia','vazia'],[-3,1,'preta','ataque'],[-2,1,'vazia','vazia'],[-1,1,'branca','defesa'],[0,1,'vazia','vazia'],[1,1,'branca','salto'],[2,1,'vazia','vazia'],[3,1,'vazia','vazia']],
[[-4,2,'vazia','vazia'],[-3,2,'vazia','vazia'],[-2,2,'vazia','vazia'],[-1,2,'vazia','vazia'],[0,2,'vazia','vazia'],[1,2,'vazia','vazia'],[2,2,'vazia','vazia']],
[[-4,3,'vazia','vazia'],[-3,3,'vazia','vazia'],[-2,3,'vazia','vazia'],[-1,3,'vazia','vazia'],[0,3,'vazia','vazia'],[1,3,'vazia','vazia']],
[[-4,4,'vazia','vazia'],[-3,4,'vazia','vazia'],[-2,4,'vazia','vazia'],[-1,4,'vazia','vazia'],[0,4,'vazia','vazia']]]).