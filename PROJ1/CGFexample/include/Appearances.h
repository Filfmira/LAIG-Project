#ifndef APPEARANCES_H
#define APPEARANCES_H

#include <vector>
#include <CGFappearance.h>
#include "Textures.h"
class Appearance:public CGFappearance{

protected:
	char* id;
	Texture *t;

public:
	
	static float texlength_s;
	static float texlength_t;
	
	Appearance(char* id):CGFappearance()
	{
		this->id=id;
		this->t = NULL;
	}

	void setTexture(Texture* t){
		this->t=t;
		this->CGFappearance::setTexture(t);

	}
	Texture* getTexture(){return this->t;}
	char* getId(){return id;}

};



#endif