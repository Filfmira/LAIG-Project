#ifndef JOGADA_H
#define JOGADA_H
#include <vector>
#include "PieceData.h"

class Jogada{
private:
	int id;
	vector<PieceData> pecasAdicionadas;
	vector<PieceData> pecasRemovidas;
	static int nextId;
	bool jogador; //branco -> true; preto -> false

public:
	Jogada(vector<PieceData> adicionadas, vector<PieceData> removidas, bool jogador);
	int getId();
	vector<PieceData> getAdicionadas();
	vector<PieceData> getRemovidas();

	void restartAnimacoes();
};

#endif