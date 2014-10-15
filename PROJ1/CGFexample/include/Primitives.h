#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "CGFobject.h"
#include <math.h>

class Primitive{
public:
	virtual void draw(){};
	virtual char* getNome(){return "gg";};

};

class Rectangle: public Primitive{
private:
	float x1;
	float x2;
	float y1;
	float y2;
public:
	Rectangle(float x1, float y1, float x2, float y2){
		this->x1 = x1;	
		this->x2 = x2;

		this->y1 = y1;	
		this->y2 = y2;
	}
		
	void draw(){
		//glRectf(x1,y1,x2,y2);

		glPushMatrix();
			
			//glMultMatrixf(tra1); 
			glNormal3d(0,0,1) ;
			
			glBegin(GL_QUADS);
				glTexCoord2d(0,0);
				glVertex3d(x1,y1,0);
				glTexCoord2d(0,1);
				glVertex3d(x2,y1,0);
				glTexCoord2d(1,1);
				glVertex3d(x2,y2,0);
				glTexCoord2d(1,0);
				glVertex3d(x1,y2,0);				
			glEnd();

		glPopMatrix();
	};

	char* getNome(){return "Rectangulo";};
};

class Triangle: public Primitive{

private:
	float x1;
	float x2;
	float x3;
	float y1;
	float y2;
	float y3;
	float z1;
	float z2;
	float z3;

public:

	Triangle(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3){

		this->x1 = x1;	
		this->x2 = x2;
		this->x3 = x3;

		this->y1 = y1;	
		this->y2 = y2;
		this->y3 = y3;

		this->z1 = z1;	
		this->z2 = z2;
		this->z3 = z3;
	}
	
	void draw(){
		glPushMatrix();			
			//glMultMatrixf(tra1); 
			glNormal3f(0,0,1) ;
			glBegin(GL_TRIANGLES);
				glVertex3d(x1,y1,z1);
				glVertex3d(x2,y2,z2);
				glVertex3d(x3,y3,z3);
			glEnd();
		glPopMatrix();
	
	};

	char* getNome(){return "Triangulo";};
};


class Cylinder: public Primitive{
private:
	float base;
	float top;
	float height;
	int slices;
	int stacks;
	GLUquadric* cylinder;

public:
	Cylinder (float base, float top, float height, int slices, int stacks){
		this->base = base;
		this->top = top;
		this->height = height;
		this->slices = slices;
		this->stacks = stacks;
		cylinder=gluNewQuadric();
	};

	void draw(){
		double alphaStep = (360.0/(float)slices)*acos(-1.0)/180.0;

		float x1,y1;
		glPushMatrix();

			//topo
			glNormal3f(0,0,1) ;
			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0, 0, height);
				for(unsigned int i = 0; i <= slices; i++){
					x1=top*cos(i* alphaStep);
					y1=top*sin(i* alphaStep);
					glVertex3f(x1,y1, height); //guardar estes senos e cossenos para não calcular???
					glTexCoord2d(x1*0.5+0.5,y1*0.5+0.5);
				}
			glEnd();

			//base
			glPushMatrix();
				glRotated(180,0,1,0);
				glNormal3f(0,0,1) ;
				glBegin(GL_TRIANGLE_FAN);
					glVertex3f(0, 0, 0);
					for(unsigned int i = 0; i <= slices; i++){
						x1=base*cos(i* alphaStep);
						y1=base*sin(i* alphaStep);
						glVertex3d(x1,y1,0); //guardar estes senos e cossenos para não calcular???
						glTexCoord2d(x1*0.5+0.5,y1*0.5+0.5);
					}
				glEnd();
			glPopMatrix();

			//corpo do cilindro
		//	glMatrixMode(GL_TEXTURE);
			gluCylinder(cylinder, base, top, height,  slices, stacks);
		gluQuadricDrawStyle(cylinder, GLU_FILL);
		gluQuadricTexture(cylinder, GLU_TRUE);	
		gluQuadricNormals(cylinder, GLU_SMOOTH);
		glPopMatrix();


	};

	char* getNome(){return "Cilindro";};
};

class Sphere: public Primitive{
private:
	float radius;
	int slices;
	int stacks;
	GLUquadric* sphere;


public:
	Sphere(float radius, int slices, int stacks){
		this->radius = radius;
		this->slices = slices;
		this->stacks = stacks;
		sphere=gluNewQuadric();
	};

	//https://www.opengl.org/wiki/Texturing_a_Sphere
	void draw(){		
		glPushMatrix();
		gluSphere(sphere, radius, slices, stacks);
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GLU_TRUE);	
		gluQuadricNormals(sphere, GLU_SMOOTH);
		
			//void gluSphere(GLUquadric*  quad,  GLdouble  radius,  GLint  slices,  GLint  stacks);
			
		glPopMatrix();
	};

	char* getNome(){return "Esfera";};
};


class Torus: public Primitive{
private:
	float innerRadius;
	float outerRadius;
	int slices;
	int loops;

public:
	Torus(float innerRadius, float outerRadius, int slices, int loops){
		this->innerRadius = innerRadius;
		this->outerRadius = outerRadius;
		this->slices = slices;
		this->loops = loops;
	};

	void draw(){		
		glPushMatrix();
			/*void glutSolidTorus(GLdouble innerRadius,
                    GLdouble outerRadius,
                    GLint nsides, GLint rings);*/
			glutSolidTorus(innerRadius, outerRadius, slices, loops);
		glPopMatrix();
	};

	char* getNome(){return "Torus";};
};


#endif
