#ifndef _DRONE_H_
#define _DRONE_H_

#include "CGFapplication.h"
#include "Primitives.h"

class Drone:public Primitive{
public:
	void draw();
	Drone(int order, int pU, int pV, GLenum compute, GLfloat* points);
private:
	int order;
	int partsU;
	int partsV;
	GLenum compute;
	GLfloat* ctrlpoints; 
	GLfloat* nrmlcompon;
	GLfloat* colorpoints;
	GLfloat* textpoints;

};

#endif