#ifndef _PIECE_H_
#define _PIECE_H_

#include <string>
#include "Primitives.h"
#include "Poligon.h"
#include "Appearances.h" //inclui tb Textures.h

class Piece: public Primitive{
private:
	//bool cor; /*branco = true; preto = false */
	//std::string tipo; /*simples, ataques, defesa, salto, expansao */
	Poligon hexagon;
	Appearance* appearanceTopos;
	Appearance* appearanceLados;
	Appearance* appearanceLadosSelected;
	vector<Texture*> texturesPecas; //texturas de todas as peças
public:
	Piece();
	void draw(bool cor, string tipo, bool selected);
	Texture* chooseTexture(bool cor, string tipo);
};




#endif