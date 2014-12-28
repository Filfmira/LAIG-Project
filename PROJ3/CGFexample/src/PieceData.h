#ifndef PIECEDATA_H
#define PIECEDATA_H
#include <iostream>
#include <string>
#include "Animation.h"
using namespace std;

class PieceData{
private:
	int x;
	int y;
	bool cor; //s� � relevante se tipo != vazia; //true = branca, false = preta.
	string tipo;
	Animation* animation;
	
public: 
	PieceData(int x, int y, bool cor, string tipo){
		this->x = x;
		this->y = y;
		this->cor = cor;
		this->tipo = tipo;
		animation=NULL;
	}
	int getX(){return x;}
	void setX(int x){this->x = x;}
	int getY(){return y;}
	void setY(int y){this->y = y;}
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