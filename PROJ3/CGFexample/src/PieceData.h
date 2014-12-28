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
	bool cor; //só é relevante se tipo != vazia; //true = branca, false = preta.
	string tipo;
	Animation* animation;
	bool animationBool;
	
public: 
	PieceData(int x, int y, bool cor, string tipo){
		this->x = x;
		this->y = y;
		this->cor = cor;
		this->tipo = tipo;
		animationBool=false;
	}
	int getX(){return x;}
	void setX(int x){this->x = x;}
	int getY(){return y;}
	void setY(int y){this->y = y;}
	bool getCor(){return cor;}
	string getTipo(){return tipo;}

	void setAnimation(Animation* anim){this->animation=anim; animationBool=true;}
	Animation* getAnimation(){return this->animation;}
	bool hasAnimation(){return animationBool;}
	void setHasAnimation(bool a){this->animationBool=a;}

	void print(){
		cout << boolalpha; 
		
		cout << "Peca -> x: " << x <<
			" ; y: " << y <<
			" ; cor: " << cor <<
			" ; tipo: " << tipo << endl;
	}
};

#endif