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