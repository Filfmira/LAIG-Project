#include "Appearances.h"

Appearance::Appearance(){};


Appearance::Appearance(char* id):CGFappearance()
{
	this->id=id;
	this->t = NULL;
}

void Appearance::setTexture(Texture* t){
	this->t=t;
	this->CGFappearance::setTexture(t);
}


Texture* Appearance::getTexture(){
	return this->t;
}


char* Appearance::getId(){
	return id;
}