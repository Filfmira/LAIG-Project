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

public:
	Jogada(vector<PieceData> adicionadas, vector<PieceData> removidas);
	int getId();
	vector<PieceData> getAdicionadas();
	vector<PieceData> getRemovidas();

	void restartAnimacoes();
};

#endif