#ifndef MARCADOR_PONTUACAO_H
#define MARCADOR_PONTUACAO_H

#include "Primitives.h"
#include <vector>

class MarcadorPontuacao{

private: 
	int pontuacao;
	Appearance* aparencia;
	vector<Texture*> textNumeros;
	Rectangle* rectangulo;

public:
	MarcadorPontuacao();
	void incPontuacao();
	void draw();
};

#endif