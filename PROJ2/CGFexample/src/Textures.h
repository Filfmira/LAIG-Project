#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>
#include <CGFtexture.h>

class Texture:public CGFtexture{

private:
	char* id;
	float texlength_s;
	float texlength_t;
	

public:
	string getFileName(){return texName;}

	Texture(){};
	Texture(char* id, char* file, float texlength_s, float texlength_t):CGFtexture((string)file){
		this->id=id;
		this->texlength_s=texlength_s;
		this->texlength_t=texlength_t;
	};


	char* getId(){return id;}
	float getTexlengtht(){ return this->texlength_t;}
	float getTexlengths(){ return this->texlength_s;}


};



#endif