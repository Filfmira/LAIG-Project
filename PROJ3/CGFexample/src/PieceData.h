#ifndef PIECEDATA_H
#define PIECEDATA_H
#include <iostream>
#include <string>
using namespace std;

class PieceData{
private:
	int x;
	int y;
	bool cor; //só é relevante se tipo != vazia; //true = branca, false = preta.
	string tipo;
public: 
	PieceData(int x, int y, bool cor, string tipo){
		this->x = x;
		this->y = y;
		this->cor = cor;
		this->tipo = tipo;
	}
	int getX(){return x;}
	int getY(){return y;}
	bool getCor(){return cor;}
	string getTipo(){return tipo;}
	void print(){
		cout << boolalpha; 
		cout << "Peca -> x: " << x <<
			" ; y: " << y <<
			" ; cor: " << cor <<
			" ; tipo: " << tipo << endl;
	}
};

#endif