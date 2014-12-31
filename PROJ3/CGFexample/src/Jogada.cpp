#include "Jogada.h"

int Jogada::nextId = 0;

Jogada::Jogada(vector<PieceData> adicionadas, vector<PieceData> removidas){
	this->id = Jogada::nextId++;
	this->pecasAdicionadas = adicionadas;
	this->pecasRemovidas = removidas;
}

int Jogada::getId(){
	return this->id;
}

vector<PieceData> Jogada::getAdicionadas(){
	return this->pecasAdicionadas;
}

vector<PieceData> Jogada::getRemovidas(){
	return this->pecasRemovidas;
}

void Jogada::restartAnimacoes(){
	for (unsigned int i = 0; i < pecasAdicionadas.size(); i++)
		pecasAdicionadas[i].restartAnimacao();
	for (unsigned int i = 0; i < pecasRemovidas.size(); i++)
		pecasRemovidas[i].restartAnimacao();
}
