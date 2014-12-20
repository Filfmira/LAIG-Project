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
	Appearance();
	Appearance(char* id);
	void setTexture(Texture* t);
	Texture* getTexture();
	char* getId();

};



#endif

