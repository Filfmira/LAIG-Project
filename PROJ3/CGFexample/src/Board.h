#ifndef BOARD_H
#define BOARD_H
#include "PieceData.h"
#include "Piece.h"
#include <vector>

using namespace std;

class Board{
private:
	vector<vector<PieceData> > board;
	vector<PieceData> historico; //jogadas feitas
	Piece piece; //instanciada uma peça, que será usada para desenhar todo o tabuleiro
public:
	Board(vector<vector<PieceData> > b);
	bool fazerJogada(PieceData jogada);
	void draw(int selectedX, int selectedY);
	vector<vector<PieceData> > getBoard();
	void setBoard(vector<vector<PieceData> > board);
	void addPiece(PieceData newPiece);
	PieceData getPiece(int x, int y);
	void addPieceHistorico(PieceData pd);
};

#endif
