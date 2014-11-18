


#include <iostream>
#include "CGFApplication.h"
#include "Primitives.h"
#include "Patch.h"
class Vehicle{
protected:
	Cylinder ponta;
	Cylinder corpo;
	Cylinder laterais;
	Patch p;
	float deg2rad;
public:
	Vehicle()
	{
		deg2rad=acos(-1.0)*180;
		int putas =( 45*acos(-1.0) ) /180.0;
		ponta=Cylinder(1.5,0,2,20,20);
		corpo=Cylinder(1.5,1.5,8,20,20);
		laterais=Cylinder(1,0.5,2,20,20);
		float lol[48] = {
			0,0,0,
			0,0,0,
			0,0,0,
			0,0,0,

			//aqui devia levantar
			1,0,-1,
			1,putas,-putas,
			1,putas,putas,
			1,0,1,

			2,0,-1,
			2,putas,-putas,
			2,putas,putas,
			2,0,1,

			3,0,0,
			3,0,0,
			3,0,0,
			3,0,0

       };
		p=Patch(3,20,20,"fill",&lol[0]);


	}
	void draw(){
		/*glPushMatrix();
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
		glPopMatrix();*/


		p.draw();
		
	}

};
