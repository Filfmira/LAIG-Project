	#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "CGFobject.h"
#include <math.h>
#include "Appearances.h"

static float distanciaPontos(float x1, float y1, float x2, float y2){
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


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
	float length;
	float height;

public:
	Rectangle();
	Rectangle(float x1, float y1, float x2, float y2){
		this->x1 = x1;	
		this->x2 = x2;
		this->y1 = y1;	
		this->y2 = y2;

		this->length = distanciaPontos(x1,y1,x2,y1);
		this->height = distanciaPontos(x1,y1,x1,y2);

	}
		
	void draw(){
		float s = this->length/Appearance::texlength_s;
		float t = this->height/Appearance::texlength_t;
		glPushMatrix();
			glNormal3d(0,0,1) ;
			glBegin(GL_QUADS);
				glTexCoord2d(0,0);
				glVertex3d(x1,y1,0);
				glTexCoord2d(s,0);
				glVertex3d(x2,y1,0);
				glTexCoord2d(s,t);
				glVertex3d(x2,y2,0);
				glTexCoord2d(0,t);
				glVertex3d(x1,y2,0);				
			glEnd();

		glPopMatrix();
	};

	char* getNome(){return "Rectangulo";};
};

class Cube: public Primitive{
private:
	Rectangle* r;

public:
	Cube(){
		r=new Rectangle(-0.5, -0.5, 0.5, 0.5);
		
	}
		
	void draw(){
		glPushMatrix();
		glTranslated(0,0,0.5);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(90,0,1,0);
		glTranslated(0,0,0.5);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(180,0,1,0);
		glTranslated(0,0,0.5);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(-90,0,1,0);
		glTranslated(0,0,0.5);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(90,1,0,0);
		glTranslated(0,0,0.5);
		r->draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(-90,1,0,0);
		glTranslated(0,0,0.5);
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
	float a,b,c,cos3,sin3;

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

			this->a=sqrt(pow(x3-x2,2)+pow(y3-y2,2)+pow(z3-z2,2)); //distancia de x2 a x3
			this->b=sqrt(pow(x1-x3,2)+pow(y1-y3,2)+pow(z1-z3,2)); //distancia de x1 a x3
			this->c=sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(z2-z1,2)); //distancia de x1 a x2
			this->cos3=(a*a-b*b+c*c)/(2*a*c);
			this->sin3=sqrt(1-pow(cos3,2));
	}

	void draw(){
		glPushMatrix();			
			glNormal3f(0,0,1) ;
			glBegin(GL_TRIANGLES);
				glTexCoord2d(0,0);
				glVertex3d(x1,y1,z1);
				glTexCoord2d(c/Appearance::texlength_s,0);
				glVertex3d(x2,y2,z2);
				glTexCoord2d( (c-a*cos3)/Appearance::texlength_s,(a*sin3)/Appearance::texlength_t);
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
	double alphaStep;

public:
	Cylinder(){};
	Cylinder (float base, float top, float height, int slices, int stacks){
		
		this->base = base;
		this->top = top;
		this->height = height;
		this->slices = slices;
		this->stacks = stacks;
		cylinder=gluNewQuadric();

		alphaStep = (360.0/(float)slices)*acos(-1.0)/180.0;
	};

	void draw(){
		

		float x1,y1;
		glPushMatrix();

			//topo
			glNormal3f(0,0,1) ;
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0.5,0.5);
				glVertex3f(0, 0, height);
				for(int i = 0; i <= slices; i++){
					x1=top*cos(i* alphaStep);
					y1=top*sin(i* alphaStep);
					glTexCoord2d(x1*0.5+0.5,y1*0.5+0.5);
					glVertex3f(x1,y1, height); 
					
				}
			glEnd();

			//base
			glPushMatrix();
				glRotated(180,0,1,0);
				glNormal3f(0,0,1) ;
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0.5,0.5);
					glVertex3f(0, 0, 0);
					for(int i = 0; i <= slices; i++){
						x1=base*cos(i* alphaStep);
						y1=base*sin(i* alphaStep);
						glTexCoord2d(x1*0.5+0.5,y1*0.5+0.5);
						glVertex3d(x1,y1,0); 
						
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
	std::vector<std::vector<float>> textCoords;
	std::vector<std::vector<float>> vertexCoords;
	std::vector<std::vector<float>> normalsCoords;
	int slices;
	int loops;

public:
	Torus(float innerRadius, float outerRadius, int slices, int loops){
		this->innerRadius = innerRadius;
		this->outerRadius = outerRadius;
		this->slices = slices;
		this->loops = loops;
		calculateCoordinates();
	};

	void draw(){		
		glBegin(GL_TRIANGLE_STRIP);
	for(unsigned int i=0; i<vertexCoords.size();i++) {
		glTexCoord2f(textCoords[i][0], textCoords[i][1]);
		glNormal3f(normalsCoords[i][0], normalsCoords[i][1], normalsCoords[i][2]);
		glVertex3f(vertexCoords[i][0], vertexCoords[i][1], vertexCoords[i][2]);
	}
	glEnd();
	};

	char* getNome(){return "Torus";};



	void calculateCoordinates() {
	 std::vector<float> vert(3);
	 std::vector<float> vNormal(3);
	 std::vector<float> text(2);
	 float pi=acos(-1.0);
	 float majorStep = 2.0f*pi / slices;   
	 float minorStep = 2.0f*pi / loops;   

	 for (int i=0; i<slices; ++i) {   
	  float a0 = i * majorStep;   
	  float a1 = a0 + majorStep;   
	  float x0 = cos(a0);   
	  float y0 = sin(a0);   
	  float x1 = cos(a1);   
	  float y1 = sin(a1);   

	  for (int j=0; j<=loops; ++j)  {   
	   float b = j * minorStep;   
	   float c = cos(b);   
	   float r = innerRadius * c + outerRadius;   
	   float z = innerRadius * sin(b);   

	   text[0] = (float) i/slices;
	   text[1] = (float) j/loops;
	   textCoords.push_back(text);
	   vNormal[0] = x0*c;   
	   vNormal[1] = y0*c;   
	   vNormal[2] = z/innerRadius;   
	   normalsCoords.push_back(vNormal);  
	   vert[0] = x0*r;
	   vert[1] = y0*r;
	   vert[2] = z;
	   vertexCoords.push_back(vert);

	   text[0] = (float) (i+1)/slices;
	   text[1] = (float) j/loops;
	   textCoords.push_back(text);
	   vNormal[0] = x1*c;   
	   vNormal[1] = y1*c;   
	   vNormal[2] = z/innerRadius;   
	   normalsCoords.push_back(vNormal);
	   vert[0] = x1*r;
	   vert[1] = y1*r;
	   vert[2] = z;
	   vertexCoords.push_back(vert);
  }
 }
}










};


#endif
