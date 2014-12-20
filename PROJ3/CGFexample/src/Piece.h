#ifndef _PIECE_H_
#define _PIECE_H_

#include <string>
#include "Primitives.h"
#include "Poligon.h"

class Piece: public Primitive{
private:
	bool cor; /*branco = true; preto = false */
	std::string tipo; /*simples, ataques, defesa, salto, expansao */
	Poligon hexagon;
public:
	Piece(bool cor, std::string tipo);
	void draw();

};




#endif