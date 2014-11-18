


#include <iostream>
#include "CGFApplication.h"
#include "Primitives.h"
class Vehicle{
protected:
	Cylinder ponta;
	Cylinder corpo;
	Cylinder laterais;
public:
	Vehicle()
	{
		ponta=Cylinder(1.5,0,2,20,20);
		corpo=Cylinder(1.5,1.5,8,20,20);
		laterais=Cylinder(1,0.5,2,20,20);
	}
	void draw(){
		glPushMatrix();
		glTranslated(0,0,8);
		ponta.draw();
		glPopMatrix();

		glPushMatrix();
		corpo.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.8,0,-0.5);
		laterais.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1.8,0,-0.5);
		laterais.draw();
		glPopMatrix();
	}

};
