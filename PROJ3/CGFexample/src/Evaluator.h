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


class Cube: public Primitive{
private:
	Plane* r;

public:
	Cube(){
		r=new Plane(10);
		
	}
		
	void draw(){
		glPushMatrix();
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(90,0,1,0);
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(180,0,1,0);
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(-90,0,1,0);
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(90,1,0,0);
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(-90,1,0,0);
		glTranslated(0,0,0.5);
		glRotated(90,1,0,0);
		r->draw();
		glPopMatrix();
	};

	char* getNome(){return "Cube";}
};


class Box: public Primitive{
private:
	Cube* cube;

public:
	Box(){
		cube=new Cube();
		
	}
		
	void draw(){
	};

	void draw(float largura,float comprimento,float altura,float expessura)	{
		glPushMatrix();
		
	
		glPushMatrix();
		glScaled(comprimento,expessura,largura);
		cube->draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(comprimento/2-(expessura/2),(expessura/2)+(altura/2),0);
		glScaled(expessura,altura,largura);
		cube->draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-comprimento/2+(expessura/2),(expessura/2)+(altura/2),0);
		glScaled(expessura,altura,largura);
		cube->draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0,(expessura/2)+(altura/2),largura/2-(expessura/2));
		glScaled(comprimento,altura,expessura);
		cube->draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0,(expessura/2)+(altura/2),-largura/2+(expessura/2));
		glScaled(comprimento,altura,expessura);
		cube->draw();
		glPopMatrix();

	glPopMatrix();

	};


	char* getNome(){return "Box";}
};



#endif