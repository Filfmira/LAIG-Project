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
	vector< vector<float>> controlPoints;

public:
	Patch(){};
	Patch(int o, int pU, int pV, string comp,vector< vector<float>> cp ){
		order=o;
		partsU=pU;
		partsV=pV;
		compute=comp;
		for(int i=0;i<cp.size();i++)
		{
			controlPoints.push_back(cp[i]);
		}
		/*for(int i=0;i<controlPoints.size();i++)
		{
			cout<<"x:"<<controlPoints[i][0]<<" y:"<<controlPoints[i][1]<<" z:"<<controlPoints[i][2]<<endl;
		}*/
	};

	void draw()
	{

		glEnable(GL_CW);
		glEnable(GL_AUTO_NORMAL);
		/*GLfloat ctrl_pts[9][3];
		for (int i =0;i<controlPoints.size();i++){
			ctrl_pts[i][0] = controlPoints.at(i).at(0);
			ctrl_pts[i][1] = controlPoints.at(i).at(1);
			ctrl_pts[i][2] = controlPoints.at(i).at(2);
		}
		*/

		GLfloat *exampleControlPoints= new GLfloat[(order+1)*3];

		for(int i =0;i<controlPoints.size();i++)
		{
			exampleControlPoints[i*3+0]=controlPoints[i][0];
			exampleControlPoints[i*3+1]=controlPoints[i][1];
			exampleControlPoints[i*3+2]=controlPoints[i][2];

		}

		
		for(int i=0;i<48;i++)
		{
			cout<<i<<":"<<exampleControlPoints[i]<<endl;
		}
		cin.get();

		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, order+1,
			0, 1, 3*(order+1), order+1, &exampleControlPoints[0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
		glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);

		glPushMatrix();
	glFrontFace(GL_CW);

	//if(compute.compare("fill") == 0)
		glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
	/*else if(compute.compare("line") == 0)
		glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
	else glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);*/
	
	glFrontFace(GL_CCW);
	glPopMatrix();
	};



};

#endif