#ifndef PATCH_H
#define PATCH_H

#include "CGFapplication.h"
#include "Primitives.h"


class Patch: public Primitive{
private:
	float u0;
	float u1;
	float v0;
	float v1;
	int order;
	int partsU;
	int partsV;
	int u_stride;
	int v_stride;
	string compute;
	GLfloat *controlPoints;

public:
	Patch(){};
	Patch(int o, int pU, int pV, string comp,vector< vector<float>> cp ){
		order=o;
		partsU=pU;
		partsV=pV;
		compute=comp;

		int kkk=((order+1)*(order+1))*3;
		controlPoints= new GLfloat[kkk];

		for(unsigned int i =0;i<cp.size();i++)
		{
			controlPoints[i*3+0]=cp[i][0];
			controlPoints[i*3+1]=cp[i][1];
			controlPoints[i*3+2]=cp[i][2];

		}
	};

	Patch(int o, int pU, int pV, string comp, float *arraycp ){
		order=o;
		partsU=pU;
		partsV=pV;
		compute=comp;

		int kkk=((order+1)*(order+1))*3;
		controlPoints= new GLfloat[kkk];
		//controlPoints=arraycp;
		for(int i =0;i<kkk;i++)
		{
			controlPoints[i]=arraycp[i];
			controlPoints[i]=arraycp[i];
			controlPoints[i]=arraycp[i];

		}
	};

	void draw()
	{

		glEnable(GL_CW);
		glEnable(GL_AUTO_NORMAL);

		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, order+1,
			0, 1, 3*(order+1), order+1, &controlPoints[0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
		glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);

		glPushMatrix();
		glFrontFace(GL_CW);

		if(compute.compare("fill") == 0)
			glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
		else if(compute.compare("line") == 0)
			glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
		else glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
	
		glFrontFace(GL_CCW);
		glPopMatrix();
	};



};

#endif