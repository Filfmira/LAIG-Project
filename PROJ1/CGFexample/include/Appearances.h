#ifndef APPEARANCES_H
#define APPEARANCES_H

#include <vector>
#include <CGFappearance.h>

class Appearance{

private:
	char* id;
	CGFappearance* appearance;
	

public:

	Appearance(char* id,CGFappearance* appearance)
	{
		this->id=id;
		this->appearance=appearance;

	}
	CGFappearance* getAppearance(){return this->appearance;}
	char* getId(){return id;}

};



#endif