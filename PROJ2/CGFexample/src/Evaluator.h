#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "CGFapplication.h"
#include "Primitives.h"


class Plane: public Primitive{

private:
	unsigned int parts;



public:
	Plane(unsigned int parts);
	void draw();
	char* getNome();
};


#endif