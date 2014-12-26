%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).
:-use_module(library(lists)).
:-ensure_loaded('Tabula_4.pl').

port(60070).

% launch me in sockets mode
server:-
	port(Port),
	socket_server_open(Port, Socket),
	socket_server_accept(Socket, _Client, Stream, [type(text)]),
	write('Accepted connection'), nl,
	serverLoop(Stream),
	socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
	repeat,
	read(Stream, ClientMsg),
	write('Received: '), write(ClientMsg), nl,
	parse_input(ClientMsg, MyReply),
	format(Stream, '~q.~n', [MyReply]),
	write('Wrote: '), write(MyReply), nl,
	flush_output(Stream),
	(ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(comando(Arg1, Arg2), Answer) :-
	comando(Arg1, Arg2, Answer).
	
parse_input(quit, ok-bye) :- !.
parse_input(novo-tabuleiro, [ok,Tabuleiro]) :- /*novoTabuleiro(9, Tabuleiro),*/ tabuleiroExemplo1(Tabuleiro), !.
parse_input([jogada, Jogada, Tabuleiro], [ok,TabuleiroNovo]):- 
	colocaPeca(Jogada, Tabuleiro, TabuleiroNovo), write('deu'), nl, !. /*colocaPeca funcionou, retorna ok */
parse_input([jogada, Jogada, Tabuleiro], [not-ok,gg]):- write('n deu'), !. /* celula estava ocupada, retorna not-ok */

comando(Arg1, Arg2, Answer) :-
	write(Arg1), nl, write(Arg2), nl,
	Answer = 5.