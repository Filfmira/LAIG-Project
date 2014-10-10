#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>
#include <CGFtexture.h>

class Texture{

private:
	char* id;
	CGFtexture* texture;
	float texlength_s;
	float texlength_t;
	

public:

	Texture(char* id, char* file, float texlength_s, float texlength_t){
		this->id=id;
		try{
			texture=new CGFtexture((string)file);
		}
		catch (int e){
			printf("ERROR, COULD NOT FOUND FILE");
		}
		//texture=new CGFtexture((string)file);
		this->texlength_s=texlength_s;
		this->texlength_t=texlength_t;
	};


	char* getId(){return id;}
	CGFtexture* getTexture(){return this->texture;}

};



#endif