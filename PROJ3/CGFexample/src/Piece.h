#ifndef _PIECE_H_
#define _PIECE_H_

#include <string>
#include "Primitives.h"
#include "Poligon.h"
#include "Appearances.h"

class Piece: public Primitive{
private:
	//bool cor; /*branco = true; preto = false */
	//std::string tipo; /*simples, ataques, defesa, salto, expansao */
	Poligon hexagon;
	Appearance* appearanceTopos;
	Appearance* appearanceLados;
public:
	Piece(std::string texturaTopos);
	void draw();

};




#endif