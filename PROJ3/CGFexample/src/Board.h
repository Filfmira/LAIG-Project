#ifndef BOARD_H
#define BOARD_H
#include "PieceData.h"
#include "Piece.h"
#include <vector>
#include "Jogada.h"

using namespace std;

class Board{
private:
	vector<vector<PieceData> > board;
	vector<Jogada> historico; //jogadas feitas
	Piece piece; //instanciada uma peça, que será usada para desenhar todo o tabuleiro
public:
	Board(vector<vector<PieceData> > b);
	bool fazerJogada(PieceData jogada);
	void draw(int selectedX, int selectedY);
	vector<vector<PieceData> > getBoard();
	void setBoard(vector<vector<PieceData> > board);
	void addPiece(PieceData newPiece);
	void removePiece(PieceData oldPiece);
	PieceData getPiece(int x, int y);
	void addJogadaHistorico(Jogada pd);
	vector<Jogada> getHistorico();
	void popBackHistorico();
	void changeTextures(int i);
};

#endif
