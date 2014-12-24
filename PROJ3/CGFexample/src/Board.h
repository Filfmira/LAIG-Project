#ifndef BOARD_H
#define BOARD_H
#include "PieceData.h"
#include "Piece.h"
#include <vector>

using namespace std;

class Board{
private:
	vector<vector<PieceData> > board;
	//guardar historico de jogadas aqui tambem
	Piece piece; //instanciada uma peça, que será usada para desenhar todo o tabuleiro
public:
	Board(vector<vector<PieceData> > b);
	bool fazerJogada(PieceData jogada);
	void draw();

};

#endif
