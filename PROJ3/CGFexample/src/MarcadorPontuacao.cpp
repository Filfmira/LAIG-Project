#include "MarcadorPontuacao.h"

MarcadorPontuacao::MarcadorPontuacao(){
	this->pontuacao = 0;
	this->aparencia = new Appearance("app");
	this->rectangulo = new Rectangle(0, 0, 1, 1);

	Texture* temp = new Texture("zero", "texturasNumeros/zero.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("um", "texturasNumeros/um.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("dois", "texturasNumeros/dois.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("tres", "texturasNumeros/tres.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("quatro", "texturasNumeros/quatro.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("cinco", "texturasNumeros/cinco.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("seis", "texturasNumeros/seis.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("sete", "texturasNumeros/sete.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("oito", "texturasNumeros/oito.png", 1, 1); this->textNumeros.push_back(temp);
	temp = new Texture("nove", "texturasNumeros/nove.png", 1, 1); this->textNumeros.push_back(temp);

	this->aparencia->setTexture(this->textNumeros[0]);
}

void MarcadorPontuacao::incPontuacao(){
	if (this->pontuacao < 9)
		this->pontuacao++;
	this->aparencia->setTexture(this->textNumeros[this->pontuacao]);
}

void MarcadorPontuacao::draw(){
	glPushMatrix();
		this->aparencia->apply();
		glScaled(2,3,1);
		this->rectangulo->draw();
	glPopMatrix();
}