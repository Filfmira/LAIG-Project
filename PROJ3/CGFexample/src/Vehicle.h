#ifndef VEHICLE_H
#define VEHICLE_H


#include <iostream>
#include "CGFApplication.h"
#include "Primitives.h"
#include "Patch.h"
class Vehicle:public Primitive{
protected:
	Cylinder ponta;
	Cylinder corpo;
	Cylinder laterais;
	Patch p;
	Patch c;
	float deg2rad;
	Appearance app;
	Texture *t;
public:
	Vehicle();
	void draw();
	void pata();
};



#endif